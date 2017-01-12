const {app, BrowserWindow} = require('electron');
const ipcMain = require('electron').ipcMain;
const path = require('path');
const url = require('url');
app.setPath('exe', '/home/mech-user/work/nekonote');
let mainWindow = null;
app.on('window-all-closed', function() {
  app.quit();
});

app.on('ready', function() {
  mainWindow = new BrowserWindow({
    width: 1000,
    height: 600
  });
  mainWindow.loadURL('file://' + __dirname + '/index.html');
  mainWindow.on('closed', function() {
    mainWindow = null;
  });
});
//
// ipcMain.on('_command', function(event, arg) {
//   console.log(arg);
//   const exec = require('child_process').exec;
//   exec('ls', function(error, stdout, stderr) {
//     event.sender.send('command_result', stdout);
//   });
//   event.sender.send('command_result', 'ahahah');
// });
