import pickle
from GUI import Application, ScrollableView, Document, Window, \
  FileType, Cursor, rgb
from GUI.Geometry import pt_in_rect, offset_rect, rects_intersect
from GUI.StdColors import black, red

class Launcher(Application):
    def __init__(self):
        Application.__init__(self)
        self.file_type = FileType(name = "Blob Document", suffix = "blob")

    def open_app(self):
        self.new_cmd()

    def make_document(self, fileref):
        return BlobDoc()

    def make_window(self,document):
        win = Window(size = (400,400), document = document)
