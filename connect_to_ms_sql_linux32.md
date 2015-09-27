<table border='0'>

<tr><td align='center'>
<b>unicode <a href='http://en.wikipedia.org/wiki/UnixODBC'>unixODBC</a> driver</b>
</td></tr>

<tr><td>
<blockquote><a href='http://mirrors.ibiblio.org/freetds/stable/'>freetds-0.91.tar.gz</a></blockquote>

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
test@ubuntu:~$ sudo ln -s /usr/lib/i386-linux-gnu/libodbc.so.1.0.0 /usr/lib/i386-linux-gnu/libodbc.so<br>
test@ubuntu:~$ sudo ldconfig<br>
test@ubuntu:~$ sudo apt-get install unixodbc-dev<br>
test@ubuntu:~/freetds-0.91$ ./configure<br>
test@ubuntu:~/freetds-0.91$ make<br>
test@ubuntu:~/freetds-0.91$ sudo make install<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>/usr/local/etc/freetds.conf</b>
</td></tr>

<tr><td><pre><code><br>
[ms_srv]<br>
<br>
host = 192.168.1.52<br>
instance = SQLEXPRESS<br>
tds version = 8.0<br>
text size = 10000000<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>/etc/odbcinst.ini</b>
</td></tr>

<tr><td><pre><code><br>
[ms_sql]<br>
<br>
Driver = /usr/local/lib/libtdsodbc.so<br>
Setup = /usr/lib/i386-linux-gnu/odbc/libtdsS.so<br>
UsageCount = 1<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>/etc/odbc.ini</b>
</td></tr>

<tr><td><pre><code><br>
[ms_dsn]<br>
<br>
Driver = ms_sql<br>
Servername = ms_srv<br>
Database = master<br>
TDS_Version = 8.0<br>
</code></pre></td></tr>

</table>