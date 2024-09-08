
// Default open Staff tab
document.addEventListener("DOMContentLoaded", function() {
    document.querySelector(".tablinks").click();
});

function openForm(evt, formName) {
    var i, tabcontent, tablinks;

    // Hide all tab content
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }

    // Remove the background color of all tab links
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }

    // Show the current tab, and add an "active" class to the button that opened the tab
    document.getElementById(formName).style.display = "block";
    evt.currentTarget.className += " active";
}


// Function to handle form submission for Admin sign-in
function validateAdminSignIn() {
    sessionStorage.setItem('role', 'admin'); // For admin
    var password = document.getElementById('admin-password').value;
    var adminError = document.getElementById('adminError');

    if (password === '123') {
        // Redirect to admin main menu
        window.location.href = 'admin-main-menu.html';
        return false; // Prevent form submission
    } else {
        adminError.textContent = 'Incorrect password. Please try again.';
        return false; // Prevent form submission
    }
}

// Function to handle form submission for Staff sign-in
function validateStaffSignIn() {
    sessionStorage.setItem('role', 'staff'); // For staff
    var username = document.getElementById('username').value;
    var password = document.getElementById('password').value;
    var staffError = document.getElementById('staffError');

    if (username === 'staff' && password === '1234') {
        // Redirect to staff main menu
        window.location.href = 'staff-main-menu.html';
        return false; // Prevent form submission
    } else {
        staffError.textContent = 'Incorrect username or password. Please try again.';
        return false; // Prevent form submission
    }
}




// Function to handle logout action
function logout() {
    // Clear the session or any authentication data
    window.location.href = 'index.html';
}

function redirectToMainMenu() {
    // Assume you store the role in sessionStorage or localStorage
    var userRole = sessionStorage.getItem('role'); // 'admin' or 'staff'

    if (userRole === 'admin') {
        window.location.href = 'admin-main-menu.html';
    } else if (userRole === 'staff') {
        window.location.href = 'staff-main-menu.html';
    } else {
        // Default to staff menu if role is not set or unknown
        window.location.href = 'staff-main-menu.html';
    }
}

document.getElementById('showRentalsBtn').addEventListener('click', function() {
    // Show the pop-up
    document.getElementById('popup').style.display = 'block';

    // Fetch rental data from the server
    fetch('fetch_records.php') // Adjust the path to your PHP script
        .then(response => response.json()) // Parse JSON response
        .then(data => {
            // Get the rental data container
            const rentalDataDiv = document.getElementById('rentalData');
            rentalDataDiv.innerHTML = ''; // Clear any previous data

            // Create an HTML table to display the data
            let table = '<table><tr><th>ID</th><th>User ID</th><th>RFID Tag</th><th>Rental Start</th></tr>';
            data.forEach(rental => {
                table += `<tr>
                    <td>${rental.id}</td>
                    <td>${rental.user_id}</td>
                    <td>${rental.rfid_tag}</td>
                    <td>${rental.rental_start}</td>
                </tr>`;
            });
            table += '</table>';
            rentalDataDiv.innerHTML = table;
        })
        .catch(error => console.error('Error fetching rental data:', error));
});

document.getElementById('closePopup').addEventListener('click', function() {
    // Hide the pop-up
    document.getElementById('popup').style.display = 'none';
});
