<table border='0'>
<tr><td align='center'>
<b>Oracle Database Instant Client Basic Lite</b>
</td></tr>

<tr><td>
<blockquote><a href='http://www.oracle.com/technetwork/database/features/instant-client/'>instantclient-basiclite-linux-11.2.0.3.0.zip</a></blockquote>

<blockquote>extract to</blockquote>

<pre><code><br>
/home/test/<br>
</code></pre>
</td></tr>

<tr><td align='center'>
<b>terminal</b>
</td></tr>

<tr><td><pre><code><br>
test@ubuntu:~$ sudo apt-get install libaio1<br>
test@ubuntu:~/instantclient_11_2$ sudo cp ./libclntsh.so.11.1 /usr/local/lib/<br>
test@ubuntu:~/instantclient_11_2$ sudo cp ./libnnz11.so /usr/local/lib/<br>
test@ubuntu:~/instantclient_11_2$ sudo cp ./libociicus.so /usr/local/lib/<br>
test@ubuntu:~/instantclient_11_2$ sudo ln -s /usr/local/lib/libclntsh.so.11.1 /usr/local/lib/libclntsh.so<br>
test@ubuntu:~$ sudo ldconfig<br>
</code></pre></td></tr>

</table>