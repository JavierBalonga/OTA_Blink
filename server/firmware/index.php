<?PHP
header('Content-type: text/plain; charset=utf8', true);

if(
    $_SERVER['HTTP_USER_AGENT'] != 'ESP8266-http-Update' ||
    !isset($_SERVER['HTTP_X_ESP8266_STA_MAC']) ||
    !isset($_SERVER['HTTP_X_ESP8266_VERSION']) ||
    !isset($_SERVER['HTTP_X_ESP8266_FREE_SPACE']) ||
    !isset($_SERVER['HTTP_X_ESP8266_SKETCH_MD5'])
) {
    header($_SERVER["SERVER_PROTOCOL"].' 403 Forbidden', true, 403);
    echo "only for ESP8266 updater! (header)\n";
    exit();
}

$path = "bin/".$_SERVER['HTTP_X_ESP8266_VERSION'];


if($_SERVER['HTTP_X_ESP8266_FREE_SPACE'] < filesize($path)) {
    header($_SERVER["SERVER_PROTOCOL"].' -100 too less space', true, -100);
    exit();
}

if($_SERVER['HTTP_X_ESP8266_SKETCH_MD5'] == md5_file($path)) {
    header($_SERVER["SERVER_PROTOCOL"].' 304 Not Modified', true, 304);
    exit();
}

header($_SERVER["SERVER_PROTOCOL"].' 200 OK', true, 200);
header('Content-Type: application/octet-stream', true);
header('Content-Disposition: attachment; filename='.basename($path));
header('Content-Length: '.filesize($path), true);
header('x-MD5: '.md5_file($path), true);
readfile($path);

?>
