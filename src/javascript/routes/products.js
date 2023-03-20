/** importing all the necessary exports for the program to use
  
 * express is the framework that we are using to create the server
 * express.router() is a method that returns a router object
 * productmodules is the file that we are importing from the module folder which contains the schema for the database
 
*/
const express = require('express');
const router = express.Router();
const productModules = require('../Model/productModel');
const bodyParser = require('body-parser');

/**
 * router.use(express.json()) to recognize the incoming Request Object as a JSON Object.
 * router.use(express.urlencoded({ extended: true })) to recognize the incoming Request Object as strings or arrays.
 */

router.use(express.json());
router.use(express.urlencoded({ extended: true }));
router.use(bodyParser.json());
router.use(bodyParser.urlencoded({ extended: false }));
/**
 * 
 * using async and await we will wait untill the request is done with the database and then send the response.
 * productModules.find() is the method that we are using to find all the documents in the database.
 * res.status(200).Json(products) is the method we are using to send the response to the client.
 * 
 * */
router.get('/', async function(req, res) {
    const products = await productModules.find();
    res.status(200).json(products);
})

/**
 * checking if the product is in the database and if it is then send the response to the client.
 *  using if(!product and product.length == 0) to check if the product is in the database.
 * 
 */
router.get('/:name', async function(req, res) {
    const name = req.params.name;
    try {
        const product = await productModules.find({ name: name });
        if (!product || product.length == 0) return res.status(404).send('The product with the given name was not found.');
        res.status(200).json(product);
    } catch (err) {
        res.status(400).json({ message: err.message })
    }
});



module.exports = router;