// server.js
const app = require("./app");
const PORT = 3000;

// Start the server
const server = app.listen(PORT, () => {
  console.log(`Server is running on http://localhost:${PORT}`);
});

// Export the server instance for testing
module.exports = server;
