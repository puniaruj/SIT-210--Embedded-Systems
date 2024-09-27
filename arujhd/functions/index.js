/**
 * Import function triggers from their respective submodules:
 *
 * const {onCall} = require("firebase-functions/v2/https");
 * const {onDocumentWritten} = require("firebase-functions/v2/firestore");
 *
 * See a full list of supported triggers at https://firebase.google.com/docs/functions
 */

// const {onRequest} = require("firebase-functions/v2/https");
// const logger = require("firebase-functions/logger");

// Create and deploy your first functions
// https://firebase.google.com/docs/functions/get-started

// exports.helloWorld = onRequest((request, response) => {
//   logger.info("Hello logs!", {structuredData: true});
//   response.send("Hello from Firebase!");
// });


const functions = require("firebase-functions"); // Import Firebase Functions
const admin = require("firebase-admin"); // Import Firebase Admin SDK

admin.initializeApp(); // Initialize the Firebase Admin SDK

// Example: A simple HTTP function to control LEDs
exports.toggleLED = functions.https.onRequest((req, res) => {
  if (req.method !== "POST") {
    return res.status(400).send("Only POST requests are accepted");
  }

  const ledCommand = req.body.led; // Read LED command from request body

  // Write the command to the Realtime Database
  admin.database().ref("/ledCommand")
      .set(ledCommand)
      .then(() => res.status(200).send("LED command set successfully"))
      .catch((error) =>
        res.status(500).send(`Error setting LED command: ${error}`),
      );
});
