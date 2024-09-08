<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "bike_rental";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("<h2 style='color: red;'>Connection failed: " . $conn->connect_error . "</h2>");
}

// Get the RFID tag and action from the URL parameters
$rfid_tag = $_GET['tag'];
$action = isset($_GET['action']) ? $_GET['action'] : 'start'; // Default to 'start' if no action is provided

if ($action == 'start') {
    // Handle rental start
    $rental_start = date("Y-m-d H:i:s");

    // Prepare the statement to find the user by RFID tag
    $stmt = $conn->prepare("SELECT id FROM users WHERE rfid_tag = ?");
    $stmt->bind_param("s", $rfid_tag);
    $stmt->execute();
    $result = $stmt->get_result();

    if ($result->num_rows > 0) {
        $user = $result->fetch_assoc();
        $user_id = $user['id'];

        // Prepare the statement to insert the rental record
        $insert_stmt = $conn->prepare("INSERT INTO rentals (user_id, rfid_tag, rental_start) VALUES (?, ?, ?)");
        $insert_stmt->bind_param("iss", $user_id, $rfid_tag, $rental_start);

        if ($insert_stmt->execute()) {
            echo "<h2 style='color: green;'>New rental record created successfully for RFID: $rfid_tag</h2>";
        } else {
            echo "<h2 style='color: red;'>Error: Could not create rental record. Please try again.</h2>";
        }

        $insert_stmt->close();
    } else {
        echo "<h2 style='color: red;'>Error: No user found with RFID tag: $rfid_tag</h2>";
    }

    $stmt->close();

} elseif ($action == 'end') {
    // Handle rental end
    $rental_end = date("Y-m-d H:i:s");

    // Update the rental record to set the rental end time
    $update_stmt = $conn->prepare("UPDATE rentals SET rental_end = ? WHERE rfid_tag = ? AND rental_end IS NULL");
    $update_stmt->bind_param("ss", $rental_end, $rfid_tag);

    if ($update_stmt->execute()) {
        if ($update_stmt->affected_rows > 0) {
            echo "<h2 style='color: green;'>Rental ended successfully for RFID: $rfid_tag</h2>";
        } else {
            echo "<h2 style='color: red;'>Error: No active rental found to end for RFID: $rfid_tag</h2>";
        }
    } else {
        echo "<h2 style='color: red;'>Error: Could not update rental record. Please try again.</h2>";
    }

    $update_stmt->close();
}

$conn->close();
?>
