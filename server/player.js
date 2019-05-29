/* eslint-disable */
var mongoose = require('mongoose');

// Mongoose allows us to define a Schema for our Mongodb
var Player = mongoose.model(`Player`, {
    name: {
      type: String,
      default: false
    },
    level: {
      type: String,
      default: false
    }
});

module.exports = {
  Player
};