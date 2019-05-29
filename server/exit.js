/* eslint-disable */
var mongoose = require('mongoose');

// Mongoose allows us to define a Schema for our Mongodb
var Exit = mongoose.model(`Exit`, {
    time: {
      type: String,
      default: false
    },
    count: {
      type: String,
      default: false
    },
    status: {
      type: String,
      default: false
    }
});

module.exports = {
  Exit
};