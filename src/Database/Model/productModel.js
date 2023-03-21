/**
 * Database Schema for product
 */

const mongoose = require('mongoose');
mongoose.set('strict', true);
/**
 * Schema
 */
const productSchema = new mongoose.Schema({
    name: String,

    address: String,
    Axis_X_high: Number,
    Axis_X_low: Number,
    Axis_Y_high: Number,
    Axis_Y_low: Number,
    Axis_Z_high: Number,
    Axis_Z_low: Number,
    Danger: String,

    DateTime: { type: Date, default: Date.now },
});

module.exports = mongoose.model('Product', productSchema);