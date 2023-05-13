"use strict";
//定义服务器的端口
var serverPort = 8080;
//创建websocket服务器对象
var websocket = require('websocket').server;
//创建http服务器对象
var http = require('http');
//创建http服务器
var server = http.createServer(function(request, response) {
    //发送http协议头部
    response.writeHead(200, {'Content-Type': 'text/plain'});
    //发送字符串
    response.write('This is a websocket server, not intended for http\n');
    //发送结束
    response.end();
});
//监听服务器的端口为前面定义的8080,如果有客户端连接到监听端口,则在终端输出服务器的端口号
server.listen(serverPort, function() {
    console.log('Server is listening on port ' + serverPort);
});
//创建websocket服务器
var wsServer = new websocket({
    //调用server服务
    httpServer: server
});

//定义connections保存连接信息
var connections = [];
//发送信息到开发板
function broadcastMessage(message) {
    for (var i = 0; i < connections.length; i++) {
        connections[i].sendUTF(message);
    }
}
//监听客户端的连接请求
wsServer.on('request', function(request) {
    /* Save the connection */
    //保存请求链接信息到connection变量中
    var connection = request.accept(null, request.origin);

    /* Store the connection in the list of connections */
    //获得链接索引
    var connectionIndex = connections.push(connection) - 1;
    //输出信息到控制台
    console.log('Connection from ' + connection.remoteAddress + '.');
    //处理开发板消息
    broadcastMessage('Connection from ' + connection.remoteAddress + '.');
    //监听客户端发送数据的请求
    connection.on('message', function(message) {
        //判断客户端发送过来的数据是文本还是二进制类型
        if (message.type === 'utf8') {
            console.log((new Date()) + ' Message received: ' +
                        message.utf8Data);
            broadcastMessage(message.utf8Data);
        }
    });

    // user disconnected
    //监听客户端关闭链接,关闭链接时输出信息到控制台
    connection.on('close', function(connection) {
        console.log((new Date()) + ' Connection lost: ' +
                    connection.remoteAddress);
        connections.splice(connectionIndex, 1);
    });

});
