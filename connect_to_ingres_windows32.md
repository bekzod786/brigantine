<table border='0'>

<tr><td align='center'>
<b>unicode <a href='http://en.wikipedia.org/wiki/ODBC'>ODBC</a> driver</b>
</td></tr>

<tr><td>
<blockquote><a href='http://esd.actian.com/platform/Windows_32-Bit/drivers/Client_Runtime/Client_Runtime'>clientruntime-10s-126-win-x86.zip</a></blockquote>

<blockquote>extract to<br>
<pre><code><br>
C:\temp\<br>
</code></pre></td></tr></blockquote>

<tr><td align='center'>
<b>command prompt as administrator</b>
</td></tr>

<tr><td><pre><code><br>
C:\temp\clientruntime-10.1.0-126-win-x86&gt;setup.exe<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>file <a href='http://en.wikipedia.org/wiki/Data_Source_Name'>DSN</a></b>
</td></tr>

<tr><td><pre><code><br>
[ODBC]<br>
<br>
DRIVER=Ingres<br>
UID=administrator<br>
PWD=<br>
SENDDATETIMEASINGRESDATE=Y<br>
DATEALIAS=INGRESDATE<br>
SERVERTYPE=INGRES<br>
DATABASE=demodb<br>
SERVER=192.168.1.152<br>
</code></pre></td></tr>

</table>