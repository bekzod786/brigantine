<table border='0'>

<tr><td align='center'>
<b>unicode <a href='http://en.wikipedia.org/wiki/ODBC'>ODBC</a> driver</b>
</td></tr>

<tr><td>
<blockquote><a href='http://www.ibm.com/search/csass/search?q=download+Informix+Client+SDK'>clientsdk.4.10.FC1DE.WIN.zip</a></blockquote>

<blockquote>extract to<br>
<pre><code><br>
C:\temp\<br>
</code></pre></td></tr></blockquote>

<tr><td align='center'>
<b>command prompt as administrator</b>
</td></tr>

<tr><td><pre><code><br>
C:\temp&gt;installclientsdk.exe<br>
</code></pre></td></tr>

<tr><td align='center'>
<b>file <a href='http://en.wikipedia.org/wiki/Data_Source_Name'>DSN</a></b>
</td></tr>

<tr><td><pre><code><br>
[ODBC]<br>
<br>
DRIVER=IBM INFORMIX ODBC DRIVER (64-bit)<br>
PRO=onsoctcp<br>
HOST=192.168.1.152<br>
SERV=9088<br>
SRVR=ol_informix1170<br>
DATABASE=sysmaster<br>
UID=informix<br>
PWD=<br>
DELIMIDENT=y<br>
</code></pre></td></tr>

</table>