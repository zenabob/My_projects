
import * as Yup from 'yup';

export function nameValidator(name) {
  const schema = Yup.string()
    .required("Full Name is required.")
    .matches(/^[A-Za-z\s]+$/, 'Only English letters');
  try {
    schema.validateSync(name);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function messageValidator(message) {
    if (!message) return "Message can't be empty."
    return ''
}

export function firstNameValidator(firstName) {
  const schema = Yup.string()
    .required("First Name is required.")
    .matches(/^[A-Za-z\s]+$/, 'Only English letters');
  try {
    schema.validateSync(firstName);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function lastNameValidator(lastName) {
  const schema = Yup.string()
    .required("Last Name is required.")
    .matches(/^[A-Za-z\s]+$/, 'Only English letters');
  try {
    schema.validateSync(lastName);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function emailValidator(email) {
  const emailRegex = /\S+@\S+\.\S+/;
  const schema = Yup.string()
    .required("Email is required.")
    .matches(emailRegex, 'Invalid email.')
    .email('Email Address is not valid.');

  try {
    schema.validateSync(email);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function phoneValidator(phone) {
    var phoneRegex = /^\(?([0-9]{3})\)?[-. ]?([0-9]{3})[-. ]?([0-9]{4})$/;
    const schema = Yup.string()
      .required("Phone number is required.")
      .matches(phoneRegex, 'Invalid phone number');
    try {
      schema.validateSync(phone);
      return '';
    } catch (error) {
      return error.message;
    }
}

export function cityValidator(city) {
  const schema = Yup.string()
    .required("City is required.");
  try {
    schema.validateSync(city);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function degreeValidator(degree) {
  const schema = Yup.string()
    .required("Degree is required.")
    .matches(/^[A-Za-z\s]+$/, 'Only English letters');
  try {
    schema.validateSync(degree);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function ContactSubjectValidator(subject) {
  const schema = Yup.string()
    .required("Subject is required.")
  try {
    schema.validateSync(subject);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function subjectValidator(subject) {
  const schema = Yup.string()
    .required("Subject is required.")
    .matches(/^[A-Za-z\s]+$/, 'Only English letters');
  try {
    schema.validateSync(subject);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function careerValidator(career) {
  const schema = Yup.string()
    .required("Career is required.")
    .matches(/^[A-Za-z\s]+$/, 'Only English letters');
  try {
    schema.validateSync(career);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function ageValidator(age) {
  const schema = Yup.string()
    .required("Date is required.");
  try {
    schema.validateSync(age);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function companyValidator(company) {
  const schema = Yup.string()
    .required("Company is required.")
    .matches(/^[A-Za-z\s]+$/, 'Only English letters');
  try {
    schema.validateSync(company);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function imageUrlValidator(imageUrl) {
  const schema = Yup.string()
    .required("Image is required.");
  try {
    schema.validateSync(imageUrl);
    return '';
  } catch (error) {
    return error.message;
  }
}

export function genderValidator(gender) {
  const schema = Yup.string()
    .required("Gender is required.");
  try {
    schema.validateSync(gender);
    return '';
  } catch (error) {
    return error.message;
  }
}
