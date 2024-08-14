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

$customer_name = $_POST['customer_name'];
$bike_id = $_POST['bike_id'];
$rental_date = $_POST['rental_date'];

$sql = "INSERT INTO rentals (customer_name, bike_id, rental_date) VALUES ('$customer_name', '$bike_id', '$rental_date')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();

