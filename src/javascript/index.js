const express = require('express');
const app = express();
const mongoose = require('mongoose');
const productModel = require('./Model/productModel');
const productRouting = require('./routes/products');
const bodyParser = require('body-parser');

app.use(express.json())
app.use(express.urlencoded({ extended: true }))
app.use("/products", productRouting);
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));

mongoose.set('strictQuery', true);
mongoose.connect('mongodb://127.0.0.1:27017')
    .then(() => { console.log("Connected to DB") })
    .catch((err) => { console.log("Error", err) });

const products = [];

app.get('/', function(req, res) {
    res.write('<!DOCTYPE html>')
    res.write('<html style="font-family: Roboto, Arial, sans-serif;">')
    res.write('<head><title>Rest api</title></head>')
    res.write('<body>')
    res.write('<p>Welcome</p>')
    res.write('</body>')
    res.write('</html>')
    res.end()
})


app.post('/', async(req, res) => {
    try {
        const name = req.body.name;
        const Axis_X_high = req.body.Axis_X_high;
        const Axis_X_low = req.body.Axis_X_low;
        const Axis_Y_high = req.body.Axis_Y_high;
        const Axis_Y_low = req.body.Axis_Y_low;

        const product = new productModel({
            name: name,
            Axis_X_high: Axis_X_high,
            Axis_X_low: Axis_X_low,
            Axis_Y_high: Axis_Y_high,
            Axis_Y_low: Axis_Y_low,
            DateTime: Date.now(),

        });
        const result = await product.save();
        res.status(200).json(result);
    } catch (error) {
        console.error(error);
        res.status(500).send('Server Error');
    }
});

app.listen(3000);