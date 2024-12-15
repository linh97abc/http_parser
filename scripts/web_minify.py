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

    @staticmethod
    def Gen(data, out):
        with io.BytesIO() as content:
            with gzip.GzipFile(fileobj=content, mode='w',
                               mtime=0,
                               compresslevel=9) as gz_obj:
                gz_obj.write(data)

            content.seek(0)

            with open(out, 'w') as f:
                for chunk in iter(lambda: content.read(8), b''):
                    f.write(MakeGzipFile.make_hex(chunk))


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


class WebMinify():
    def __init__(self, filePath: str):
        self.filePath = filePath
        self.ext = os.path.splitext(self.filePath)[1]
        self.minifier = None
        if (self.ext == '.html'):
            self.minifier = HtmlMinify(filePath)
        elif (self.ext == '.css'):
            self.minifier = CssMinify(filePath)
        elif (self.ext == '.js'):
            self.minifier = JsMinify(filePath)
        else:
            self.minifier = RawMinify(filePath)

        tmp_path = re.sub(r'[\\/]', '_',  self.filePath).strip()
        self.out_path = tmp_path

    def Gen(self, out: str):
        data = self.minifier.MinifyBin()
        out_path = os.path.join(out, self.GetOutFileName())
        MakeGzipFile.Gen(data, out_path)

    def GetOutVarName(self):
        return self.out_path.replace('.', '_')

    def GetOutFileName(self):
        return self.out_path + '.inc'


class WebSourceGenGzip():
    def __init__(self, out: str, list_file):
        self.outPath = out
        self.list_file = list_file

    def Gen(self):
        with open(os.path.join(self.outPath, "webSrc.h"), 'w') as f:
            for i in self.list_file:
                webMinify = WebMinify(i)
                webMinify.Gen(self.outPath)

                f.write(
                    f"static const char {webMinify.GetOutVarName()}[] = " +
                    '{\n#include "' +
                    webMinify.GetOutFileName() +
                    '"\n};\n')


WebSourceGenGzip('generated', [
    'index.html',
    'content.css',
    'control.css',
    'control.js',
    'firmware.css',
    'fixheight.js',
    'log.css',
    'menu.css',
    'page.css',
    'page.js',
]).Gen()
