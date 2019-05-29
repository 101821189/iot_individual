/* eslint-disable */
var mongoose = require('mongoose');


// Mongoose allows us to define a Schema for our Mongodb
var Fan = mongoose.model(`Fan`, {
    status: {
      type: String,
      default: false
    },
    temp: {
      type: String,
      default: false
    }
});

module.exports = {
    Fan
};