function fixHeight()
{
    banner = document.getElementById("menu");
    var height = window.innerHeight - banner.offsetHeight - 10;

    // control = document.getElementById("control");
    content = document.getElementById("content")
    // control.style.height = height + 'px'
    content.style.height = height + 'px'

    logcontent = document.getElementById("logContent")
    var logHeight = height - 96;
    logcontent.style.height = logHeight + 'px'
}


window.onresize = function (event) {
    fixHeight()

}

// window.onload = function ()
// {
//     fixHeight()
// };