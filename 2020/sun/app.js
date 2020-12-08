var express = require('express');
var app = express();
const mysql = require("mysql");
var bodyParser = require('body-parser');
var async = require('async');
const session = require('express-session');
const e = require('express');

app.use(bodyParser.urlencoded({ extended: true }));

app.set('view engine', 'ejs');
app.engine('html', require('ejs').renderFile);
app.use(express.json());
app.use(express.static('views'));
app.use(express.static('node_modules'));
app.use(session({
  key: 'sid',
  secret: 'secret',
  resave: false,
  saveUninitialized: true,
  cookie: {
    maxAge: 24000 * 60 * 60 // 쿠키 유효기간 24시간
  }
}));

var id="";
var http = require('http').createServer(app)
var io = require('socket.io')(http);

http.listen(3000,function(){
    console.log('server start');
   
})

io.on('connection', (socket)=>{
  socket.on("data", function(data){ //data는 배열로
    
    var d = new Date();
    var date = `${d.getFullYear()}-${d.getMonth()+1}-${d.getDate()}`;  
    connection.query(`insert into datas values('${date}', ${d.getHours()}, ${data[0]}, ${data[1]}, ${data[2]}, ${data[3]}, '${id}')`);
    console.log(data);
  
  })
})
  


var connection = mysql.createConnection({
    user : 'root',
    password : '  ',
    port: 3307,
    database : '  '
});
// connection.connect();


app.get('/',function(req,res){
  res.render('index.html');
})

app.get('/test',function(req,res){
  console.log(req.session.name);
  var result;
  if(req.session.uid){
    result = [true, req.session.name];
  }
  else
    result=[false];
  res.send(result);
})

app.get("/logout", function(req,res,next){
  req.session.destroy();
  res.clearCookie('sid');
  res.redirect('index.html');
});

app.post('/login',function(req,res){
  var inputid = req.body.inputid;
  var inputpw = req.body.inputpw;
  connection.query(`select * from user where id='${inputid}'and passwd='${inputpw}'`, function(err,rows){
    if(err)
      console.log(err);
    else{
      id = inputid;
      console.log(id);
      req.session.uid = inputid;
      req.session.name = rows[0].name;
      
      res.redirect('index.html');
    }
  })
})

app.post('/join',function(req,res){
  var id = req.body.inid;
  var pw = req.body.inpw;
  var name = req.body.inname;
  connection.query(`insert into user values('${id}','${pw}','${name}') `, function(err,rows){
    if(err){
      console.log(err);
      res.send(false);
    }
    else{
      res.send(true);
    }
  })
})


app.post('/showlog', function(req, res){
    var date = req.body.date;
    console.log(date);
    console.log(req.session.uid);
    connection.query(`select * from datas where date='${date}' and userid='${req.session.uid}'`, function(err, rows){
      console.log(rows);
      res.send(rows);
    })
  }
)
