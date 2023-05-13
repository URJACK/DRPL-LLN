var http = require("http"); 
var fs = require("fs");  
var url = require('url'); 

 
var ws = require("nodejs-websocket");  
 
/**net在服务器端创建客户端*/
var net = require('net');

var dgram = require('dgram');

var serverSocket = dgram.createSocket('udp4');
UDPPort=12345
var topmsg='';
var TempGet='0';
//创建websocket服务器  
var server = ws.createServer();

server.on('connection',function (conn) {  
    console.log("connection sussess");  
    conn.on("text", function (str) {  
        server.connections.forEach(function (connection) { 
            connection.sendText(str); 
            console.log(str)

            console.log('服务器:' + str);
        });  
    });  
});  
server.listen(8080);




//udp服务器
serverSocket.on('message', function(msg, rinfo){
  //console.log('recv %s(%d bytes) from client %s:%d\n', msg, msg.length, rinfo.address, rinfo.port);

  //echo to client
  //serverSocket.send(msg, 0, msg.length, rinfo.port, rinfo.address);
  topmsg = msg.toString();
  var a=topmsg.substring(4,6)
  var b=topmsg.substring(0,4)
  if((b=="Tem:") && (a !="00"))
  {
    //console.log(topmsg)
    console.log(a)
    TempGet=a;
    server.connections.forEach(function (connection) {  
        connection.sendText(a); 
        console.log('服务器:' + a);
    });  
  }
  
});

//    err - Error object, https://nodejs.org/api/errors.html
serverSocket.on('error', function(err){
  console.log('error, msg - %s, stack - %s\n', err.message, err.stack);
});

serverSocket.on('listening', function(){
  console.log("echo server is listening on port 7.");
})

serverSocket.bind(UDPPort);


 //创建服务器  
    http.createServer(function(request,response) {  
        //解析请求，包括文件名  
        var pathname= url.parse(request.url).pathname;  
        //输出请求的文件名  
        console.log("Request for "+ pathname + "  received.");  
      
        //从文件系统中都去请求的文件内容  
        fs.readFile(pathname.substr(1),function(err, data) {  
            if(err) {  
                console.log(err);  
                //HTTP 状态码 404 ： NOT FOUND  
                //Content Type:text/plain  
                response.writeHead(404,{'Content-Type': 'text/html'});  
            }  
            else {  
                //HTTP 状态码 200 ： OK  
                //Content Type:text/plain  
                response.writeHead(200,{'Content-Type': 'text/html'});  
      
                //写会相应内容  
                response.write(data.toString()); 
            }  
            //发送响应数据  
            response.end();  
        });  
    }).listen(8000);  
      
    console.log('Server running at http://127.0.0.1:8000/index.html');  


