<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "bike_rental";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT customer_name, bike_id, rental_date FROM rentals ORDER BY rental_date DESC LIMIT 20";
$result = $conn->query($sql);

$records = [];
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        $records[] = $row;
    }
}

echo json_encode($records);

$conn->close();


