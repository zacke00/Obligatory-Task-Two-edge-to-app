const mongoose = require('mongoose');

const productModules = require('./Model/productModel');

mongoose.set('strictQuery', true);
mongoose.connect('mongodb://127.0.0.1:27017')
    .then(() => console.log('Connected to MongoDB...'))
    .catch(err => console.error('Could not connect to MongoDB...'));
/**
 * create a test product to initialize the database
 */
async function createProduct() {

    const product = new productModules({
        name: 'test',
        Axis_X_high: 0.5,
        Axis_X_low: -0.5,
        Axis_Y_high: 0.5,
        Axis_Y_low: -0.5,
        Axis_Z_high: 0.5,
        Axis_Z_low: -0.5,
        DateTime: Date.now(),
    });

    const result = await product.save();
    console.log(result);
}

createProduct();