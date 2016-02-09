function get(url) {
    // Return a new promise.
    return new Promise(function(resolve, reject) {
        // Do the usual XHR stuff
        var req = new XMLHttpRequest();
        req.open('GET', url);

        req.onload = function() {
            // This is called even on 404 etc
            // so check the status
            if (req.status == 200) {
                // Resolve the promise with the response text
                //setTimeout(function(){ resolve(req.response); }, 200);
                resolve(req.response);
            }
            else {
                // Otherwise reject with the status text
                // which will hopefully be a meaningful error
                reject(Error(req.statusText));
            }
        };

        // Handle network errors
        req.onerror = function() {
            reject(Error("Network Error"));
        };

        // Make the request
        req.send();
    });
}

function getJSON(url) {
    return get(url).then(JSON.parse);
}

function getTSV(url) {
    return get(url).then(d3.tsv.parse).then(function(data) {
        data.forEach(function(d) {
            d.n = +d.n;
            d.mean = +d.mean;
            d.stddev = +d.stddev;
            d.max = +d.max;
            d.min = +d.min;
        });

        return data;
    });
}

var chartdiv = d3.select(".chartdiv");
var tsvroot = chartdiv.attr("data-tsvroot") || ".";

getJSON(tsvroot + '/main.json').then(function(mainjson) {
    mainjson.tasks.reduce(function(seq, a) {
        return seq.then(function() {
            return makechart(a);
        });
    }, Promise.resolve()).then(function() {
        console.log("Load done", mainjson);
        mainjson.tasks.forEach(drawchart);
    });
});

function makechart(task)
{
    console.log("makechart n:" + task.tn + " name: " + task.name);
    return task.algs.map(function(a) {
        return { p : getTSV(tsvroot + '/' + a.tsv), alg:a };
    }).reduce(function(seq, a) {
        return seq.then(function() {
            return a.p;
        }).then(function(d) {
            a.alg.tsvdata = d;
        });
    }, Promise.resolve()).then(function() {
        console.log("All done", task.algs);
    }).catch(function(err) {
        console.log("Argh, broken: " + err.message);
    }).then(function() {
        //console.log("Last");
    });
}


function drawchart(task) {
    var margin = {top: 20, right: 20, bottom: 30, left: 50};
    var svgwidth = chartdiv.attr("width") || 900;
    var svgheight = svgwidth/3*2;
    var width = svgwidth - margin.left - margin.right;
    var height = svgheight - margin.top - margin.bottom;
    var bisectx = d3.bisector(function(d) { return d.n; }).left;

    var maxx = d3.max(task.algs, function(a) {
        return d3.max(a.tsvdata, function(d) {
            return d.n;
        });
    });
    var maxy = d3.max(task.algs, function(a) {
        return d3.max(a.tsvdata, function(d) {
            return d.mean;
        });
    });

    var miny = d3.min(task.algs, function(a) {
        return d3.min(a.tsvdata, function(d) {
            return d.mean;
        });
    });

    var aset = d3.set();
    task.algs.map(function (a) {
        a.tsvdata.map(function(d) {
            aset.add(d.n);
        });
    });
    var allx = aset.values().sort(function(a,b){ return +a - b;});
    var firststep = allx[1] - allx[0];
    var xdomain = [ firststep/2, maxx];

    var i = 0;
    var yticks = task.algs[0].tsvdata.map( function(d) {
        i += 1;
        return Math.ceil(maxy) / i;
    }).reverse();

    var x, y, xAxis, yAxis;
    if (task.scale == "log")
    {
        x = d3.scale.log().base(2)
            .range([0, width])
            .domain(xdomain);

        y = d3.scale.log().base(2)
            .range([height, 0])
            .domain([miny/2, Math.ceil(maxy)]);

        xAxis = d3.svg.axis()
            .scale(x)
            .orient("bottom")
            .ticks(allx.length, ",.1s");

        yAxis = d3.svg.axis()
            .scale(y)
            .orient("left")
            .ticks(yticks.length)
            .tickValues(yticks);
    }
    else if (task.scale == "linear")
    {
        x = d3.scale.linear()
            .range([0, width])
            .domain(xdomain); // TODO: [0, maxx]?

        y = d3.scale.linear()
            .range([height, 0])
            .domain([0, maxy]); // TODO: [0, maxy]?

        xAxis = d3.svg.axis()
            .scale(x)
            .orient("bottom")
            .ticks(allx.length, ",s"); // TODO: specify ticks?

        yAxis = d3.svg.axis()
            .scale(y)
            .orient("left");
    }

    if (task.plot_type == "bars")
    {
        x = d3.scale.ordinal()
            .rangeRoundBands([0, width], .1)
            .domain(task.algs.map(function(d) { return d.an; }));

        xAxis = d3.svg.axis()
            .scale(x)
            .orient("bottom");

        task.algs = task.algs.map(function(d) { d.tsvdata[0].n = d.an; return d; });
    }

    var line = d3.svg.line()
        .x(function(d) { return x(d.n); })
        .y(function(d) { return y(d.mean); });

    chartdiv
        .append("h2").attr("class", "taskname").text(task.name);

    var svg = chartdiv
        .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
        /*.style("float", "left")*/
        .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    var ltable = chartdiv
        .append("table")
        .attr("class", "tinfo")
        .attr("width", svgwidth);

    //chartdiv.append("br").attr("class", "clear");

    var color = d3.scale.category10();
    color.domain(task.algs.map(function(a) { return a.name; } ));

    svg.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis)
        .append("text")
        .attr("x", width)
        .attr("y", -12)
        .attr("dy", ".71em")
        .style("text-anchor", "end")
        .text(task.xlabel);

    svg.append("g")
        .attr("class", "y axis")
        .call(yAxis)
        .append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 6)
        .attr("dy", ".71em")
        .style("text-anchor", "end")
        .text(task.ylabel);

    var ebar = svg.selectAll(".gebar")
        .data(task.algs)
        .enter().append("g")
        .attr("class", "gebar")
        .style("stroke-opacity", 0.6)
        .style("stroke", function(d) { return color(d.name); });

    var ebar_lines = ebar.selectAll(".ebar")
        .data(function(a) { return a.tsvdata; });

    function filter_errorbar(d)
    {
        // relying on errorbar height
        var h = Math.abs( y(d.mean+d.stddev/2) - y(d.mean-d.stddev/2) );
        return h > 5;
    }

    ebar_lines.enter()
        .append("svg:line")
        .filter(filter_errorbar)
        .attr("class", "ebar")
        .attr("x1", function(d) { return x(d.n); })
        .attr("x2", function(d) { return x(d.n); })
        .attr("y1", function(d) { return y(d.mean-d.stddev/2); })
        .attr("y2", function(d) { return y(d.mean+d.stddev/2); });

    ebar_lines.enter()
        .append("svg:line")
        .filter(filter_errorbar)
        .attr("class", "ebar")
        .attr("x1", function(d) { return x(d.n)-3; })
        .attr("x2", function(d) { return x(d.n)+3; })
        .attr("y1", function(d) { return y(d.mean+d.stddev/2); })
        .attr("y2", function(d) { return y(d.mean+d.stddev/2); });

    ebar_lines.enter()
        .append("svg:line")
        .filter(filter_errorbar)
        .attr("class", "ebar")
        .attr("x1", function(d) { return x(d.n)-3; })
        .attr("x2", function(d) { return x(d.n)+3; })
        .attr("y1", function(d) { return y(d.mean-d.stddev/2); })
        .attr("y2", function(d) { return y(d.mean-d.stddev/2); });


    if (task.plot_type == "bars")
    {
        var abar = svg.selectAll(".abar")
            .data(task.algs)
            .enter().append("g")
            .attr("class", function(d) { return "abar " + d.common_filename; });

        abar
            .append("rect")
            .attr("class", "fbar")
            .style("fill", function(a) { return color(a.name); })
            .attr("x", function(d) { return x(d.an); })
            .attr("width", x.rangeBand())
            .attr("y", function(d) { return y(d.tsvdata[0].mean); })
            .attr("height", function(d) { return height - y(d.tsvdata[0].mean); });

        abar
            .append("rect")
            .attr("class", "sbar")
            .style("fill", "none")
            .style("stroke", function(a) { return d3.rgb(color(a.name)).darker(); })
            .attr("x", function(d) { return x(d.an); })
            .attr("width", x.rangeBand())
            .attr("y", function(d) { return y(d.tsvdata[0].mean); })
            .attr("height", function(d) { return height - y(d.tsvdata[0].mean); });

        ebar
            .attr("transform", "translate(" + x.rangeBand()/2 + ", 0)")

    }
    else
    {
        var apath = svg.selectAll(".apath")
            .data(task.algs)
            .enter().append("g")
            .attr("class", function(d) { return "apath " + d.common_filename; });

        apath.append("path")
            .attr("class", "line")
            .attr("d", function(a) { return line(a.tsvdata); })
            .style("stroke", function(a) { return color(a.name); });

        var gdot = svg.selectAll(".gdot")
            .data(task.algs)
            .enter().append("g")
            .attr("class", "gdot")
            .style("fill", function(d) { return color(d.name); });

        gdot.selectAll(".dot")
            .data(function(a) { return a.tsvdata; })
            .enter().append("circle")
            .attr("class", "dot")
            .attr("r", 2)
            .attr("cx", function(d) { return x(d.n); })
            .attr("cy", function(d) { return y(d.mean); });
    }

    var focus = svg.append("g")
        .attr("class", "focus")
        .style("display", "none");

    /*
    var circles = focus.selectAll('circle')
        .data(task.algs)
        .enter()
        .append('circle')
        .attr('class', 'circle')
        .attr('r', 4)
        .attr('fill', 'none')
        .attr('stroke', function (d) { return color(d.name); });
    */

    svg.append("rect")
        .attr("class", "overlay")
        .attr("width", width)
        .attr("height", height)
        .on("mouseover", function() { focus.style("display", null); })
        .on("mouseout", function() { /*focus.style("display", "none");*/ })
        .on("mousemove", mousemove)
        .on("click", mouseclick);

    var fline = focus.append("svg:line")
        .attr("class", "focus_line")
        .attr("x1", 10).attr("x2", 10)
        .attr("y1", 0).attr("y2", height);

    var pin_line = focus.append("svg:line")
        .attr("class", "pin_line")
        .style("display", "none")
        .attr("x1", 10).attr("x2", 10)
        .attr("y1", 0).attr("y2", height);

    var legend = svg.append("g")
        .attr("class", "legend")
        .attr("transform", "translate(12, 0)");

    var theader = ltable.append("tr");
    theader.append("th").text("Color");
    theader.append("th").text("Name");
    theader.append("th").text("Time").style("width", "90px");
    theader.append("th").text("RSD");
    theader.append("th").text("Status");
    theader.append("th").text("Percent of slowest");
    theader.append("th").text("Multiplier");

    updateLegend(0, -1);

    function alg_by_name(a, name)
    {
        var i = a.length;
        while (i--) {
            if (a[i].name === name)
                return a[i];
        }
        return null;
    }

    function updateLegend(cur, pin)
    {
        var new_algs = task.algs.filter(function(a) { return a.tsvdata.length > cur; });
        new_algs.sort(function (a, b) {
            return b.tsvdata[cur].mean - a.tsvdata[cur].mean;
        });
        new_algs.forEach(function(e, index, arr) {
            arr[index].index = index;
        });

        var litem = legend.selectAll(".litem")
            .data(new_algs, function(d){ return d.name; });

        litem.select(".label")
            .text(function(d) { return d.name; });

        litem.transition().duration(200)
            .attr("transform", function(d, i) { return "translate(0," + d.index * 12 + ")"; })
            .style("fill-opacity", 1);


        var lenter = litem.enter()
        .append("g")
            .attr("class", "litem")
            .style("fill-opacity", 0.1)
            .attr("transform", function(d, i) { return "translate(10," + d.index * 12 + ")"; });

        lenter
        .append("rect")
            .attr("x", 18)
            .attr("width", 18)
            .attr("height", 8)
            .style("fill", function(d) { return color(d.name); });

        lenter
        .append("text")
            .attr("class", "label")
            .attr("x", 40)
            .attr("y", 4)
            .attr("dy", ".35em")
            .text(function(d) { return d.name; });


        lenter
            .transition().duration(200)
            .attr("transform", function(d, i) { return "translate(0," + d.index * 12 + ")"; })
            .style("fill-opacity", 1);

        litem.exit()
            .transition().duration(200)
            .attr("transform", function(d, i) { return "translate(-10," + d.index * 12 + ")"; })
            .style("fill-opacity", 0.1)
            .remove();


/*******************************************************************
 *                 table legend                                    *
 *******************************************************************/
        var titem = ltable.selectAll(".titem")
            .data(new_algs, function(d){ return d.name; });


        var tenter = titem.enter()
            .append("tr")
            .attr("class", "titem")
            .on("mouseover", function(d) { d3.select("."+d.common_filename).select("path, .sbar").classed("highlighted", true); })
            .on("mouseout", function(d) { d3.select("."+d.common_filename).select("path, .sbar").classed("highlighted", false); })
            .on("mousemove", function(d) {});

        titem.classed({"exited": false});

        tenter.append("td").attr("class", "td1").append("div").attr("class", "divtd1");
        tenter.append("td").attr("class", "td2");
        tenter.append("td").attr("class", "td3");
        tenter.append("td").attr("class", "td4");
        tenter.append("td").attr("class", "td5");
        tenter.append("td").attr("class", "td6").append("div").attr("class", "percentdiv");
        tenter.append("td").attr("class", "td7");

        function percent_from_top(d, i) {
            return Math.floor( new_algs[i].tsvdata[cur].mean / new_algs[0].tsvdata[cur].mean * 100 );
        }

        titem.select(".divtd1")
            .style("background-color", function(d) { return color(d.name); })
            .style("width", "18px")
            .style("height", "10px");
        titem.select(".td2").text(function(d) { return d.name; });
        function cur_mean(d) {
            return d.tsvdata.length > cur ? d.tsvdata[cur].mean : d.tsvdata[d.tsvdata.length-1].mean;
        }
        function cur_stddev(d) {
            return d.tsvdata.length > cur ? d.tsvdata[cur].stddev : d.tsvdata[d.tsvdata.length-1].stddev;
        }
        titem.select(".td3").text(function(d) { return cur_mean(d); });
        titem.select(".td4").text(function(d) {
            return Math.round((cur_stddev(d)/cur_mean(d))*100*100)/100 + "%";
        });
        titem.select(".td5").text(function(d) { return d.status; });
        titem.select(".percentdiv")
            .style("width", function(d, i) { return percent_from_top(d, i) + "px" })
            .style("background-color", function(d) { return color(d.name); })
            .text(percent_from_top);
        titem.select(".td7").text(function(d, i) {
            if (pin >= 0 &&
                d.tsvdata.length > cur &&
                alg_by_name(new_algs, d.name).tsvdata.length > pin) {
                var cur_mean = d.tsvdata[cur].mean,
                    pin_mean = alg_by_name(new_algs, d.name).tsvdata[pin].mean;
                if (cur_mean < pin_mean)
                    return -(pin_mean/cur_mean).toFixed(2);
                else
                    return (cur_mean/pin_mean).toFixed(2);
            }
            else {
                return "";
            }
        });

        titem.exit()
            .classed({"exited":true});
    }


    var old_ii = 0,
        pin_ii = -1;
    function get_i(container) {
        var x0 = x.invert(d3.mouse(container)[0]),
            i = d3.bisectLeft(allx, x0, 1),
            d0 = allx[i - 1],
            d1 = allx[i],
            ii = x0 - d0 > d1 - x0 ? i : i-1;
        return ii;
     }

    function mousemove() {
        var ii = get_i(this);
        if (ii != old_ii)
        {
            fline.attr("x1", x(allx[ii])).attr("x2", x(allx[ii]));
            updateLegend(ii, pin_ii);
        }
        old_ii = ii;
    }

    function mouseclick() {
        pin_ii = get_i(this);
        pin_line
            .style("display", null)
            .attr("x1", x(allx[pin_ii])).attr("x2", x(allx[pin_ii]));
        updateLegend(old_ii, pin_ii);
    }

}



