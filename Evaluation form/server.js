const express = require('express');
const bodyParser = require('body-parser');
const { check, validationResult } = require('express-validator');
const mongoose = require('mongoose');
const path = require('path'); //to use the path module

const app = express();
const port = 8080;

// Connect to MongoDB
mongoose
  .connect("mongodb+srv://zenabobedat2001:fbHRdLITGIFilRMJ@cluster0.yejyt6r.mongodb.net/test?retryWrites=true&w=majority", {})
  .then(() => console.log("Db connected"))
  .catch((err) => console.log(err.message));

// Serve static files from the "Evaluation form" directory
app.use(express.static(path.join(__dirname, 'Evaluation form')));

// Schema
const studentSchema = new mongoose.Schema({
  firstName: {
    type: String,
    required: true,
  },
  lastName: {
    type: String,
    required: true,
  },
  age: {
    type: Number,
    required: true,
  },
  courses: {
    type: Array,
    required: true,
  },
  city: {
    type: String,
    required: true,
  },
  message: String, 
});

// Model
const Student = mongoose.model("Form", studentSchema);

// Middleware to parse URL-encoded data
app.use(bodyParser.urlencoded({ extended: true }));

app.get('/', (req, res) => {
  // Send the HTML file when the root URL is accessed
  res.sendFile(path.join(__dirname, 'form.html'));
});

app.post('/register', [
  // Validation checks using express-validator
  check('firstName').notEmpty().withMessage('firstName is required').isAlpha().withMessage('Invalid first name format'),
  check('lastName').notEmpty().withMessage('lastName is required').isAlpha().withMessage('Invalid last name format'),
  check('city').notEmpty().withMessage('city is required').isAlpha().withMessage('Invalid city format'),
  check('age').notEmpty().withMessage('age is required').isInt({ min: 18, max: 99 }).withMessage('Invalid age'),
], async (req, res) => {
  const errors = validationResult(req);

  if (!errors.isEmpty()) {
    return res.status(400).json({ errors: errors.array() });
  }

  const { firstName, lastName, age, courses, city, message } = req.body;

  const student = new Student({
    firstName,
    lastName,
    age,
    city,
    courses,
    message,
  });

  try {
    // Use await to handle the asynchronous save operation
    await student.save();
    console.log('Student details saved successfully');
    return res.status(200).json({ success: true, message: 'Registration Successful' });
  } catch (err) {
    console.error('Error saving student details:', err);
    return res.status(500).json({ success: false, message: 'Internal Server Error' });
  }
});

app.listen(port, () => {
  console.log('Server is running on port ' + port);
});
