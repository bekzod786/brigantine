<table border='0'>

<tr><td align='center'>
<b>unicode <a href='http://en.wikipedia.org/wiki/UnixODBC'>unixODBC</a> driver</b>
</td></tr>

<tr><td>
<blockquote><a href='http://www.ibm.com/Search/?q=download+latest+ODBC+driver'>ibm_data_server_driver_for_odbc_cli_linuxx64_v10.1.tar.gz</a></blockquote>

<blockquote>extract to<br>
<pre><code><br>
/home/test/<br>
</code></pre>
</td></tr></blockquote>

<tr><td align='center'>
<b>terminal</b>
</td></tr>

<tr><td><pre><code><br>
test@ubuntu:~$ sudo apt-get install unixodbc<br>
test@ubuntu:~$ sudo ln -s /usr/lib/x86_64-linux-gnu/libodbc.so.1.0.0 /usr/lib/x86_64-linux-gnu/libodbc.so<br>
test@ubuntu:~$ sudo ldconfig<br>
</code></pre></td></tr>

<tr><td align='center'><b>/home/test/odbc_cli/clidriver/cfg/db2cli.ini</b>
</td></tr>

<tr><td><pre><code><br>
[db2_dsn]<br>
<br>
PROTOCOL = TCPIP<br>
HOSTNAME = 192.168.1.152<br>
PORT = 50000<br>
DATABASE = SAMPLE<br>
UID = DB2ADMIN<br>
PWD =<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>/etc/odbcinst.ini</b>
</td></tr>

<tr><td><pre><code><br>
[db2]<br>
<br>
Driver64 = /home/test/odbc_cli/clidriver/lib/libdb2o.so.1<br>
Setup64 = /home/test/odbc_cli/clidriver/lib/libdb2o.so.1<br>
UsageCount = 1<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>/etc/odbc.ini</b>
</td></tr>

<tr><td><pre><code><br>
[db2_dsn]<br>
<br>
Driver = db2<br>
</code></pre></td></tr>

</table>