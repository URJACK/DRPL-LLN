var ws = null;  
function getWs() {  
    ws = new WebSocket("ws://127.0.0.1:8080");
    
    ws.onopen = function () {  
    console.log("ws open");  
    }  

    ws.onmessage = function (event) {  
         var m = parseInt(event.data);
          m=m+30;
         //ws.send(event.data);
         console.log("ws:"+m);
         var option = myChart.getOption();
         option.series[0].data = [m];   
         myChart.setOption(option); 
   
         }  

    ws.onclose = function () {  
         console.log("ws close");  
        }  
    ws.onerror = function () {  
         console.error("ws error");  
        }     
}   

