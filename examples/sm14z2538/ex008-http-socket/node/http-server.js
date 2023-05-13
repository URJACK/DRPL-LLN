//http是内置模块、直接引入  
var http = require('http');  
//创建http服务器,并监听8080端口   
http.createServer(function (req, res) {  
  //发送http头信息
  res.writeHead(200, {'Content-Type': 'text/plain'}); 
  //输出hell world到浏览器 
  res.end('Hello World\n');  
}).listen(8080);  
//输出信息到控制台   
console.log('Server running on port 8080.');  