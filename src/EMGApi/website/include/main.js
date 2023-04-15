// max samples for dygraph
var maxSamples = 60;

// relative path to the sensor data for get/post:
var serverPath = "/sensor/:80";

// callback when the Web page has been loaded
$(document).ready(function() {
    var data = [];
    var g = new Dygraph(document.getElementById("div_g"), data,
            {
                drawPoints: true,
                labels: ['Time', 'Temperature'],
            });

    $("#driver").click(function(event){
    var d = 
                {
        emg_data: 2,
        steps: 5,
        hello: "This is the base EMG value"
        };
    $.ajax({
        type: 'POST',
                url: serverPath,
                data: JSON.stringify(d),
        contentType: "application/json",
        dataType: 'json'
    });
        });
    
    window.intervalId = setInterval(function() {
    // callback for interval timer for every second
    $.getJSON(serverPath,function(result){
                data = [];
        y = result.lastvalue;
                document.getElementById("emg_data").innerHTML = Math.round(y * 100) / 100;
                for(let i = 0; i < result.time.length; i++) {
        const d = new Date();
        d.setUTCSeconds(result.time[i]/1000,result.time[i]%1000);
        var y = result.emg_data[i];
        data.push([d, y]);
        }
        g.updateOptions( { 'file': data } );
    });
    }, 1000);
});