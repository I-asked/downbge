# ##### BEGIN GPL LICENSE BLOCK #####
#
#  This program is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software Foundation,
#  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ##### END GPL LICENSE BLOCK #####

# <pep8-80 compliant>

from __future__ import absolute_import
import bpy
from bpy.types import Operator
from bpy.props import StringProperty


class EditExternally(Operator):
    """Edit image in an external application"""
    bl_idname = "image.external_edit"
    bl_label = "Image Edit Externally"
    bl_options = set(['REGISTER'])

    filepath = StringProperty(
            subtype='FILE_PATH',
            )

    @staticmethod
    def _editor_guess(context):
        import sys

        image_editor = context.user_preferences.filepaths.image_editor

        # use image editor in the preferences when available.
        if not image_editor:
            if sys.platform[:3] == "win":
                image_editor = ["start"]  # not tested!
            elif sys.platform == "darwin":
                image_editor = ["open"]
            else:
                image_editor = ["gimp"]
        else:
            if sys.platform == "darwin":
                # blender file selector treats .app as a folder
                # and will include a trailing backslash, so we strip it.
                image_editor.rstrip('\\')
                image_editor = ["open", "-a", image_editor]
            else:
                image_editor = [image_editor]

        return image_editor

    def execute(self, context):
        import os
        import subprocess

        filepath = self.filepath

        if not filepath:
            self.report(set(['ERROR']), "Image path not set")
            return set(['CANCELLED'])

        if not os.path.exists(filepath) or not os.path.isfile(filepath):
            self.report(set(['ERROR']),
                        "Image path %r not found, image may be packed or "
                        "unsaved" % filepath)
            return set(['CANCELLED'])

        cmd = self._editor_guess(context) + [filepath]

        try:
            subprocess.Popen(cmd)
        except:
            import traceback
            traceback.print_exc()
            self.report(set(['ERROR']),
                        "Image editor not found, "
                        "please specify in User Preferences > File")

            return set(['CANCELLED'])

        return set(['FINISHED'])

    def invoke(self, context, event):
        import os
        sd = context.space_data
        try:
            image = sd.image
        except AttributeError:
            self.report(set(['ERROR']), "Context incorrect, image not found")
            return set(['CANCELLED'])

        if image.packed_file:
            self.report(set(['ERROR']), "Image is packed, unpack before editing")
            return set(['CANCELLED'])

        if sd.type == 'IMAGE_EDITOR':
            filepath = image.filepath_from_user(sd.image_user)
        else:
            filepath = image.filepath

        filepath = bpy.path.abspath(filepath, library=image.library)

        self.filepath = os.path.normpath(filepath)
        self.execute(context)

        return set(['FINISHED'])


class SaveDirty(Operator):
    """Save all modified textures"""
    bl_idname = "image.save_dirty"
    bl_label = "Save Dirty"
    bl_options = set(['REGISTER', 'UNDO'])

    def execute(self, context):
        unique_paths = set()
        for image in bpy.data.images:
            if image.is_dirty:
                if image.packed_file:
                    if image.library:
                        self.report(set(['WARNING']),
                                    "Packed library image: %r from library %r"
                                    " can't be re-packed" %
                                    (image.name, image.library.filepath))
                    else:
                        image.pack(as_png=True)
                else:
                    filepath = bpy.path.abspath(image.filepath,
                                                library=image.library)
                    if "\\" not in filepath and "/" not in filepath:
                        self.report(set(['WARNING']), "Invalid path: " + filepath)
                    elif filepath in unique_paths:
                        self.report(set(['WARNING']),
                                    "Path used by more than one image: %r" %
                                    filepath)
                    else:
                        unique_paths.add(filepath)
                        image.save()
        return set(['FINISHED'])


class ProjectEdit(Operator):
    """Edit a snapshot of the view-port in an external image editor"""
    bl_idname = "image.project_edit"
    bl_label = "Project Edit"
    bl_options = set(['REGISTER'])

    _proj_hack = [""]

    def execute(self, context):
        import os

        EXT = "png"  # could be made an option but for now ok

        for image in bpy.data.images:
            image.tag = True

        # opengl buffer may fail, we can't help this, but best report it.
        try:
            bpy.ops.paint.image_from_view()
        except RuntimeError, err:
            self.report(set(['ERROR']), str(err))
            return set(['CANCELLED'])

        image_new = None
        for image in bpy.data.images:
            if not image.tag:
                image_new = image
                break

        if not image_new:
            self.report(set(['ERROR']), "Could not make new image")
            return set(['CANCELLED'])

        filepath = os.path.basename(bpy.data.filepath)
        filepath = os.path.splitext(filepath)[0]
        # fixes <memory> rubbish, needs checking
        # filepath = bpy.path.clean_name(filepath)

        if bpy.data.is_saved:
            filepath = "//" + filepath
        else:
            tmpdir = context.user_preferences.filepaths.temporary_directory
            filepath = os.path.join(tmpdir, "project_edit")

        obj = context.object

        if obj:
            filepath += "_" + bpy.path.clean_name(obj.name)

        filepath_final = filepath + "." + EXT
        i = 0

        while os.path.exists(bpy.path.abspath(filepath_final)):
            filepath_final = filepath + ("%.3d.%s" % (i, EXT))
            i += 1

        image_new.name = bpy.path.basename(filepath_final)
        ProjectEdit._proj_hack[0] = image_new.name

        image_new.filepath_raw = filepath_final  # TODO, filepath raw is crummy
        image_new.file_format = 'PNG'
        image_new.save()

        filepath_final = bpy.path.abspath(filepath_final)

        try:
            bpy.ops.image.external_edit(filepath=filepath_final)
        except RuntimeError, re:
            self.report(set(['ERROR']), str(re))

        return set(['FINISHED'])


class ProjectApply(Operator):
    """Project edited image back onto the object"""
    bl_idname = "image.project_apply"
    bl_label = "Project Apply"
    bl_options = set(['REGISTER'])

    def execute(self, context):
        image_name = ProjectEdit._proj_hack[0]  # TODO, deal with this nicer

        try:
            image = bpy.data.images[image_name, None]
        except KeyError:
            import traceback
            traceback.print_exc()
            self.report(set(['ERROR']), "Could not find image '%s'" % image_name)
            return set(['CANCELLED'])

        image.reload()
        bpy.ops.paint.project_image(image=image_name)

        return set(['FINISHED'])
