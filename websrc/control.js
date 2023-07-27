

function OnLedBtnClick(event) {
    // console.log("OnLedBtnClick")
    var classList = event.srcElement.classList;

    if (classList.contains('RequestYellowLed')) {
        classList.remove('RequestYellowLed');
        classList.add('YellowLed');
    } else if (classList.contains('GreenLed')) {
        classList.remove('GreenLed');
        classList.add('ErrLed');
    } else if (classList.contains('ErrLed')) {
        classList.remove('ErrLed');
        classList.add('GreenLed');
    } else if (classList.contains('YellowLed')) {
        classList.remove('YellowLed');
    } else {
        classList.add('GreenLed');
    }
}

function OnLedBtnPress(event) {
    // console.log("OnLedBtnPress")
    // console.log(event)
    // console.log(event.srcElement)
    // console.log(event.srcElement.classList)

    var classList = event.srcElement.classList;

    if (classList.contains('GreenLed')) {
        // console.log("dbclick g")
    } else if (classList.contains('ErrLed')) {
        // console.log("dbclick e")
    } else if (classList.contains('YellowLed')) {
        // console.log("dbclick y")
    } else {
        classList.add('RequestYellowLed');
    }
}


function Init_LedBtn() {
    var elements = document.getElementsByClassName("LedBtn");


    for (var i = 0; i < elements.length; i++) {
        var pressTimer;

        elements[i].onmouseup = function () {
            clearTimeout(pressTimer)
        }

        elements[i].onmousedown = function (event) {
            pressTimer = window.setTimeout(OnLedBtnPress, 1000, event)
        }

        elements[i].addEventListener('click', OnLedBtnClick, false);



        // elements[i].classList.remove('GreenLed');
        // elements[i].classList.remove('ErrLed');
        // elements[i].classList.remove('YellowLed');
    }
}

function OnDOBtnClick(event) {
    // if (event.srcElement.classList.contains('ActiveBtn')) {
    //     event.srcElement.classList.remove('ActiveBtn');
    // } else {
    //     event.srcElement.classList.add('ActiveBtn');
    // }
}

function Init_DOBtn() {
    var elements = document.getElementsByClassName("DOBtn");


    for (var i = 0; i < elements.length; i++) {
        elements[i].addEventListener('click', OnDOBtnClick, false);

        elements[i].classList.remove('ActiveBtn');
    }
}

function SetActiveBtn(el, isActive) {
    // console.log('SetActiveBtn')
    // console.log(el)
    // console.log(el.classList)

    el.classList.remove('ActiveBtn');

    if (isActive) {
        el.classList.add('ActiveBtn')
    }

}

const ActionMode = {
    Config: 0,
    Free: 1,
    Connect: 2,
    SelfTest: 3,
    Update: 4
}

function EnableDO(isEnable) {
    var elements = document.getElementById('DOPanel').getElementsByClassName("DOBtn");

    for (let x of elements) {
        // console.log(x.disabled)
        x.disabled = !isEnable;
    }
}

function EnableLedBtn(isEnable) {
    var elements = document.getElementById('LedPanel').getElementsByClassName("LedBtn");

    for (let x of elements) {
        // console.log(x.disabled)
        x.disabled = !isEnable;
    }
}



function InitModeDropBox() {
    var element = document.getElementById('modeDropBox')

    var OnModeDropBoxChange = function () {
        if (element.value == ActionMode.Config) {
            EnableDO(false)
            EnableLedBtn(true)
        } else if (element.value == ActionMode.SelfTest) {
            // console.log('selftest')
            EnableDO(true)
            EnableLedBtn(true)
        } else if (element.value == ActionMode.Free) {
            // console.log('Free')
            EnableDO(false)
            EnableLedBtn(true)
        } else if (element.value == ActionMode.Connect) {
            // console.log('Connect')
            EnableDO(false)
            EnableLedBtn(false)
        } else {
            EnableDO(false)
            EnableLedBtn(false)
        }


    }

    OnModeDropBoxChange();

    element.onchange = OnModeDropBoxChange
}


const LogLevelList = [
    'ALL',
    'DEBUG',
    'INFO',
    'WARN',
    'ERROR',
    'CRITICAL'
]

const LogLevelStyleClassList = {
    'WARN': 'LogWarn',
    'ERROR': 'LogErr',
    'CRITICAL': 'LogCritical'
}


function UpdateLog(log) {
    var logContentView = document.getElementById('logContent')
    var logFilterSelect = document.getElementById('logFilterSelect')

    var isAtBottom = false;

    if ((logcontent.offsetHeight + Math.round(logContentView.scrollTop)) >= logContentView.scrollHeight) {
        // you're at the bottom of the page
        isAtBottom = true;
    }

    var logLine = logContentView.getElementsByTagName("p")
    var isFullLog = false;
    if (logLine.length > 500) {
        isFullLog = true;
    }
    
    const lines = log.split(/\r?\n/);

    // console.log(lines)
    for (var line of lines) {
        // console.log(line)
        if (isFullLog) {
            logLine[0].remove()
        }
        logContentView.innerHTML += `<p> ${line} </p>`

        var newLine = logLine[logLine.length - 1];
        if (newLine.innerText.indexOf('WARN') >= 0) {
            newLine.classList.add(LogLevelStyleClassList.WARN)
        } else if (newLine.innerText.indexOf('ERROR') >= 0) {
            newLine.classList.add(LogLevelStyleClassList.ERROR)
        } else if (newLine.innerText.indexOf('CRITICAL') >= 0) {
            newLine.classList.add(LogLevelStyleClassList.CRITICAL)
        }

        if (logFilterSelect.value != 0) {
            var logLevel = LogLevelList[logFilterSelect.value]

            // console.log(newLine)

            if (newLine.innerText.indexOf(logLevel) >= 0) {
                
            } else {
                
                newLine.style.display = 'none';
            }
        }

    }
    

    if (isAtBottom)
    {

    logContentView.scrollTo(0, logContentView.scrollHeight)
    }
}

function ParserSttResID(id, data) {
    // console.log(data)


    // console.log(el)
    if ('di' in data) {
        UpdateLog(`1:1:1 DEBUG    - di: ${data.di}`)
        UpdateLog(`1:1:1 INFO     - di: ${data.di}`)
        UpdateLog(`1:1:1 WARN     - di: ${data.di}`)
        UpdateLog(`1:1:1 ERROR    - di: ${data.di}`)
        UpdateLog(`1:1:1 CRITICAL - di: ${data.di}`)
        
        var DIRow = document.getElementById("DIPanel").getElementsByClassName(id)[0]
        
        // console.log(DIRow)
        for (var i = 1; i <= 6; i++) {
            var diBtn = DIRow.getElementsByClassName(`di${i}`)[0]

            
            SetActiveBtn(diBtn, data.di & (1 << i))
        }

    }
}

function ParserSttResLog(data) {
    var el = document.getElementById('logContent').innerHTML;

    el += data
}


function ParserSttRes(data) {
    if ('q1' in data) {
        ParserSttResID('q1', data.q1)
    }

    if ('q2' in data) {
        ParserSttResID('q2', data.q2)
    }

    if ('q3' in data) {
        ParserSttResID('q3', data.q3)
    }

    if ('q4' in data) {
        ParserSttResID('q4', data.q4)
    }

    if ('q5' in data) {
        ParserSttResID('q5', data.q5)
    }

    if ('q6' in data) {
        ParserSttResID('q6', data.q6)
    }

    if ('ts' in data) {
        document.getElementById('timeStamp').innerHTML = data.ts
    }

    if ('log' in data) {
        ParserSttResLog(data.log)
    }
}

function StartGetStatus()
{
    let xhr = new XMLHttpRequest();

    xhr.responseType = 'json';

    function ReGetStatus() {
        xhr.open('GET', '/api/stt')
        xhr.send()
    }



    xhr.onload = function() {
        if (xhr.status != 200) {
            alert(`Error ${xhr.status}: ${xhr.statusText}`);
        } else {
            res = xhr.response
            // console.log(res);

            ParserSttRes(res);
        
            ReGetStatus();
        }
    };
    
    ReGetStatus();

}
// document.getElementById('modeDropBox').onchange = function (event) {
//     alert("modeDropBox")
// }

// Init_LedBtn()
// Init_DOBtn()
// InitModeDropBox()
function LogScrollUp()
{
    // alert('up')
    var logContentView = document.getElementById('logContent')
    logContentView.scrollTo(0, 0)
}

function LogScrollDown()
{
    // alert('down')
    var logContentView = document.getElementById('logContent')
    logContentView.scrollTo(0, logContentView.scrollHeight)
}

function LogFilter(logLevel)
{
    // const LogLevel = {
    //     'ALL': 0,
    //     'DEBUG': 1,
    //     'INFO': 2,
    //     'WARN': 3,
    //     'ERROR': 4,
    //     'CRITICAL': 5
    // }

    // alert('down')
    var logContentView = document.getElementById('logContent')
    var logLine = logContentView.getElementsByTagName("p")

    if (logLevel == 0) {
        for (var line of logLine) {
            line.style.display = 'block';
        }
    } else {

        const logLevelStr = LogLevelList[logLevel]

        for (var line of logLine)
        {
            // console.log(line)
            if (line.innerText.indexOf(logLevelStr) >= 0)
            {
                line.style.display = 'block';
            }
            else {
                line.style.display = 'none';
            }
        }
    }




    // logContentView.scrollTo(0, logContentView.scrollHeight)
}