import enum
import struct
import rjsmin
import csscompressor
from lxml import etree, html
import os
import re
import argparse
import codecs
import gzip
import io


class MakeGzipFile():
    @staticmethod
    def get_nice_string(list_or_iterator):
        return ", ".join("0x" + str(x) for x in list_or_iterator)

    @staticmethod
    def make_hex(chunk):
        hexdata = codecs.encode(chunk, 'hex').decode("utf-8")
        hexlist = map(''.join, zip(*[iter(hexdata)] * 2))
        return (MakeGzipFile.get_nice_string(hexlist) + ',\n')

    def GenData(data):
        with io.BytesIO() as content:
            with gzip.GzipFile(fileobj=content, mode='w',
                               mtime=0,
                               compresslevel=9) as gz_obj:
                gz_obj.write(data)

            content.seek(0)

            return content.read()


class WebFileMinify():
    def __init__(self, file: str):
        self.file = file

    def MinifyBin(self) -> bytes:
        data = b''
        with open(self.file, 'rb') as f:
            data = f.read()

        return self.Minify(data.decode()).encode()

    def Minify(self, input: str) -> str:
        pass


class RawMinify(WebFileMinify):
    def MinifyBin(self):
        data = b''
        with open(self.file, 'rb') as f:
            data = f.read()

        return data


class HtmlMinify(WebFileMinify):
    def Minify(self, input: str):
        parser = html.HTMLParser(remove_comments=True)
        tree = html.fromstring(input, parser=parser)
        # Chuyển về chuỗi HTML
        cleaned_html = etree.tostring(
            tree, method="html", pretty_print=False, encoding="unicode")
        # Xóa khoảng trắng thừa
        minified_html = re.sub(r'\s+', ' ', cleaned_html).strip()
        return minified_html


class CssMinify(WebFileMinify):
    def Minify(self, input):
        return csscompressor.compress(input)


class JsMinify(WebFileMinify):
    def Minify(self, input):
        return rjsmin.jsmin(input)


def MinifyBin(filePath: str) -> bytes:

    ext = os.path.splitext(filePath)[1]
    minifier = None
    if (ext == '.html'):
        minifier = HtmlMinify(filePath)
    elif (ext == '.css'):
        minifier = CssMinify(filePath)
    elif (ext == '.js'):
        minifier = JsMinify(filePath)
    else:
        minifier = RawMinify(filePath)

    return minifier.MinifyBin()


class WebExt(enum.Enum):
    unknow = 0
    html = 1
    css = 2
    js = 3
    csv = 4
    xml = 5
    jpg = 6
    png = 7
    gif = 8
    json = 9
    zip = 10
    pdf = 11
    mp4 = 12
    webm = 13


class WebFS():
    def __init__(self, out_dir, list_files):
        self.c_dumy_file = os.path.join(out_dir, 'webfs.inc.c')
        self.out_path = os.path.join(out_dir, 'webfs.inc')
        self.list_files = list_files

    @staticmethod
    def GetExtByName(ext: str):
        ext_to_name = {
            '.html': WebExt.html,
            '.css': WebExt.css,
            '.js': WebExt.js,
            '.csv': WebExt.csv,
            '.xml': WebExt.xml,
            '.jpg': WebExt.jpg,
            '.png': WebExt.png,
            '.gif': WebExt.gif,
            '.json': WebExt.json,
            '.zip': WebExt.zip,
            '.pdf': WebExt.pdf,
            '.mp4': WebExt.mp4,
            '.webm': WebExt.webm,
        }

        if ext in ext_to_name:
            return ext_to_name[ext]
        return WebExt.unknow

    def Gen(self):
        with open(self.c_dumy_file, 'w') as f:
            f.write('\n')

        with io.BytesIO() as out_data:
            for i in self.list_files:
                f_content = MinifyBin(i)
                f_content = MakeGzipFile.GenData(f_content)
                self.AddFileContent(out_data, i, f_content)

            out_data.seek(0)
            with open(self.out_path, 'w') as f:
                for chunk in iter(lambda: out_data.read(8), b''):
                    f.write(MakeGzipFile.make_hex(chunk))

            if False:
                out_data.seek(0)
                with open(self.out_path + '.bin', 'wb') as f:
                    f.write(out_data.read())

    def AddFileContent(self, stream, f_name: str, f_content: bytes):
        ext = os.path.splitext(f_name)[1]
        h = struct.pack('IB',
                        len(f_content),
                        __class__.GetExtByName(ext).value)

        data = f_name.encode() + b'\x00' + h + f_content
        stream.write(data)


# listFiles = [
#     'index.html',
#     'test.html',
#     'css/content.css',
#     'css/control.css',
#     'css/firmware.css',
#     'css/log.css',
#     'css/menu.css',
#     'css/page.css',
#     'js/control.js',
#     'js/fixheight.js',
#     'js/page.js',
# ]

# WebFS('generated', listFiles).Gen()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Web file minifier and gzip generator")
    parser.add_argument('--output_dir', type=str, required=True, help='Output directory for the generated files')
    parser.add_argument('input_files', type=str, nargs='+', help='List of input files to be processed')
    args = parser.parse_args()

    print(f"Generate webfs fileFS to {args.output_dir}")
    WebFS(args.output_dir, args.input_files).Gen()
