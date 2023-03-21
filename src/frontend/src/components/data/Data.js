import { useEffect, useState,React } from "react";

let products= null;
let Axis_X_High_value = null;
let Axis_X_Low_value = null;
let Axis_Y_High_value = null;
let Axis_Y_Low_value = null;
let Axis_Z_High_value = null;
let Axis_Z_Low_value = null;
let danger_level= "green";

function Data() {
    const [data, setData] = useState(null);
    
    useEffect(() => {
        
        //Date : 2023-03-21T14:05:58.096Z
        fetch("http://localhost:5000/products")
        .then((res) => res.json())
        .then((data) => setData(data));
    }, []);
    
    if(data){

        
        
        products = data.map((product) => {
            Axis_X_High_value = product.Axis_X_high;
            Axis_X_Low_value = product.Axis_X_low;
            Axis_Y_High_value = product.Axis_Y_high;
            Axis_Y_Low_value = product.Axis_Y_low;
            Axis_Z_High_value = product.Axis_Z_high;
            Axis_Z_Low_value = product.Axis_Z_low;

            product.dateTime.split("T")

            
            
            if (product.Danger == "Worst Case"){
                danger_level = "red";
            } else if (product.Danger == "Extreme"){
                danger_level = "orange";
            } else if (product.Danger == "Heavy"){
                danger_level = "yellow";
            } else if (product.Danger == "Calm"){
                danger_level = "chartreuse";
            } else if(product.Danger == "No Danger"){
                danger_level = "lime";
            }
            if(product.Danger == "Worst Case"){
                return (
                    <div>
            <section classname="container">
            <div className="" key={product.id}
            style={{background: danger_level}}>
            <h1>Elevator: {product.name}</h1>
            <p className="Axis_X_High">Highest X: {product.Axis_X_high.toFixed(2)}</p>
            <p className="Axis_X_Low">Lowest X: {product.Axis_X_low.toFixed(2)}</p>
            <p className="Axis_Y_High">Highest Y: {product.Axis_Y_high.toFixed(2)}</p>
            <p className="Axis_Y_Low">Lowest Y: {product.Axis_Y_low.toFixed(2)}</p>
            <p className="Axis_Z_High">Highest Z: {product.Axis_Z_high.toFixed(2)}</p>
            <p className="Axis_Z_Low">Lowest Z: {product.Axis_Z_low.toFixed(2)}</p>
            <p classname="Date_Time">Date : {product.DateTime}</p>
            <p >Danger level = {product.Danger}</p>
            </div>
            </section>
            </div>
        );
            }
    });  
}
  return (
    <div>
      <h1>Data</h1>
      
        {products}
    </div>
  );
}


export default Data;