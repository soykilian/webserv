<?php
http_response_code(404);
header('Content-Type: text/html; charset=UTF-8');

echo "<html>";
echo "<head><title>Page Not Found</title></head>";
echo "<body>";
echo "<h1>404 Not Found</h1>";
echo "<p>The page you are looking for could not be found.</p>";
echo "</body>";
echo "</html>";
?>
