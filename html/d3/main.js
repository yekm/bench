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

getJSON('main.json').then(function(mainjson) {
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
    return task.algs.reduce(function(seq, a) {
        return seq.then(function() {
            return getTSV(a.tsv);
        }).then(function(d) {
            a.tsvdata = d;
        });
    }, Promise.resolve()).then(function() {
        console.log("All done", task.algs);
    }).catch(function(err) {
        consile.log("Argh, broken: " + err.message);
    }).then(function() {
        //console.log("Last");
    });
}


function drawchart(task) {
    var margin = {top: 20, right: 20, bottom: 30, left: 50};
    var width = 1000 - margin.left - margin.right;
    var height = 800 - margin.top - margin.bottom;
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
    var allx = aset.values();

    var i = 0;
    var yticks = task.algs[0].tsvdata.map( function(d) {
        i += 1;
        return Math.ceil(maxy) / i;
    }).reverse();

    var x = d3.scale.log().base(2)
        .range([0, width])
        .domain([1, maxx]);

    var y = d3.scale.log().base(2)
        .range([height, 0])
        .domain([miny/2, Math.ceil(maxy)]);

    var xAxis = d3.svg.axis()
        .scale(x)
        .orient("bottom");

    var yAxis = d3.svg.axis()
        .scale(y)
        .orient("left")
        .ticks(yticks.length)
        .tickValues(yticks);

    var line = d3.svg.line()
        .x(function(d) { return x(d.n); })
        .y(function(d) { return y(d.mean); });

    d3.select("body")
        .append("h3").attr("class", "taskname").text(task.name);

    var svg = d3.select("body")
        .append("svg")
        .attr("width", width + margin.left + margin.right)
        .attr("height", height + margin.top + margin.bottom)
        .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

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

    var apath = svg.selectAll(".apath")
        .data(task.algs)
        .enter().append("g")
        .attr("class", "apath");

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

    var ebar = svg.selectAll(".gebar")
        .data(task.algs)
        .enter().append("g")
        .attr("class", "gebar")
        .style("stroke-opacity", 0.6)
        .style("stroke", function(d) { return color(d.name); });

    var ebar_lines = ebar.selectAll(".ebar")
        .data(function(a) { return a.tsvdata; });

    ebar_lines.enter()
        .append("svg:line")
        .attr("class", "ebar")
        .attr("x1", function(d) { return x(d.n); })
        .attr("x2", function(d) { return x(d.n); })
        .attr("y1", function(d) { return y(d.mean-d.stddev/2); })
        .attr("y2", function(d) { return y(d.mean+d.stddev/2); });

    ebar_lines.enter()
        .append("svg:line")
        .attr("class", "ebar")
        .attr("x1", function(d) { return x(d.n)-3; })
        .attr("x2", function(d) { return x(d.n)+3; })
        .attr("y1", function(d) { return y(d.mean+d.stddev/2); })
        .attr("y2", function(d) { return y(d.mean+d.stddev/2); });

    ebar_lines.enter()
        .append("svg:line")
        .attr("class", "ebar")
        .attr("x1", function(d) { return x(d.n)-3; })
        .attr("x2", function(d) { return x(d.n)+3; })
        .attr("y1", function(d) { return y(d.mean-d.stddev/2); })
        .attr("y2", function(d) { return y(d.mean-d.stddev/2); });


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
        .on("mouseout", function() { focus.style("display", "none"); })
        .on("mousemove", mousemove);


    var fline = focus.append("svg:line")
        .attr("x1", 10).attr("x2", 10)
        .attr("y1", 0).attr("y2", height);

    var legend = svg.append("g")
        .attr("class", "legend")
        .attr("transform", "translate(12, 0)");

    updateLegend(0);

    function updateLegend(cur)
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
            .text(function(d) {
                var mean = d.tsvdata.length > cur ? d.tsvdata[cur].mean : d.tsvdata[d.tsvdata.length-1].mean;
                return d.name + ': ' + mean;
            });

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
            .text(function(d) { return d.name + ': ' + d.tsvdata[cur].mean; });


        lenter
            .transition().duration(200)
            .attr("transform", function(d, i) { return "translate(0," + d.index * 12 + ")"; })
            .style("fill-opacity", 1);

        litem.exit()
            .transition().duration(200)
            .attr("transform", function(d, i) { return "translate(-10," + d.index * 12 + ")"; })
            .style("fill-opacity", 0.1)
            .remove();
    }


    var old_ii = 0;
    function mousemove() {
        var x0 = x.invert(d3.mouse(this)[0]),
            i = d3.bisectLeft(allx, x0, 1),
            d0 = allx[i - 1],
            d1 = allx[i],
            ii = x0 - d0 > d1 - x0 ? i : i-1;

        if (ii != old_ii)
        {
            fline.attr("x1", x(allx[ii])).attr("x2", x(allx[ii]));
            updateLegend(ii);
        }
        old_ii = ii;
    }

}



