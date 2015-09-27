<table border='0'>

<tr><td align='center'>
<b>unicode <a href='http://en.wikipedia.org/wiki/ODBC'>ODBC</a> driver</b>
</td></tr>

<tr><td>
<blockquote><a href='http://www.ibm.com/Search/?q=download+latest+ODBC+driver'>ibm_data_server_driver_for_odbc_cli_win32_v10.1.zip</a></blockquote>

<blockquote>extract to<br>
<pre><code><br>
C:\ProgramsRegistered\<br>
</code></pre>
</td></tr></blockquote>

<tr><td align='center'>
<b>command prompt as administrator</b>
</td></tr>

<tr><td><pre><code><br>
C:\ProgramsRegistered\clidriver\bin&gt;db2oreg1.exe -i<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>file <a href='http://en.wikipedia.org/wiki/Data_Source_Name'>DSN</a></b>
</td></tr>

<tr><td><pre><code><br>
[ODBC]<br>
<br>
DRIVER = IBM DATA SERVER DRIVER for ODBC - C:/ProgramsRegistered/clidriver<br>
PROTOCOL = TCPIP<br>
HOSTNAME = 192.168.1.152<br>
PORT = 50000<br>
DATABASE = TEST<br>
UID = DB2INST1<br>
PWD =<br>
</code></pre></td></tr>

</table>