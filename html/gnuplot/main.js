var req = new XMLHttpRequest();
req.open('GET', 'main.json');

req.onload = function() {
    if (req.status == 200) {
        j = JSON.parse(req.response);
        j['tasks'].map(do_task);
    }
    else {
        alert(req.statusText);
    }
};

req.onerror = function() {
    alert("Network Error");
};

req.send();

function do_task(json)
{
    var div = document.createElement('div');
    div.id = json['name'];
    document.body.appendChild(div);
    json['algs'].map( function (a) {
        var img = document.createElement('img');
        img.src = a['common_filename'] + '.svg';
        img.className = 'chart';
        img.className += ' task' + json['tn'];
        img.className += ' alg' + json['an'];
        //img.id = 'chart';
        div.appendChild(img);
    });
    var img = document.createElement('img');
    img.src = 'allinone_task' + json['tn'] + '.svg';
    img.className = 'bigchart';
    div.appendChild(img);
}
