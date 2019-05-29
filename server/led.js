/* eslint-disable */
var mongoose = require('mongoose');

// ONLY WHEN THE LED TURNS OFF, DUE TO NO MOTION WILL WE STORE TO THE DATABASE

// Mongoose allows us to define a Schema for our Mongodb
var Led = mongoose.model(`Led`, {
    duration: {
      type: String,
      default: false
    },
    date: {
      type: String,
      default: false
    }
});

module.exports = {
  Led
};