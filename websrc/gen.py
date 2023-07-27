def gen(fileName: str,):
    with open(fileName, 'r') as f:
        content = f.read()
        content = content.replace('\\', '\\\\').replace('\n', '\\n').replace('"', '\\"')

        with open(fileName + '.c', 'w') as des:
            varName = fileName.replace('.', '_')
            des.write(f'char {varName}[] = "{content}";\n')

gen('index.html')
gen('content.css')
gen('control.css')
gen('control.js')
gen('firmware.css')
gen('fixheight.js')
gen('log.css')
gen('menu.css')
gen('page.css')
gen('page.js')