const mongoose = require('mongoose');
mongoose.set('strict', true);

const productSchema = new mongoose.Schema({
    name: String,
    Axis_X_high: Number,
    Axis_X_low: Number,
    Axis_Y_high: Number,
    Axis_Y_low: Number,

    DateTime: { type: Date, default: Date.now },
});

module.exports = mongoose.model('Product', productSchema);