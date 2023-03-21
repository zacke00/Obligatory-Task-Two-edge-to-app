import { useEffect, useState, React } from "react";
import "../../assets/styles/css/Timer.css";

let timer = null;

const Timer = () => {
    const [refreshCount, setRefreshCount] = useState(0);
    const [ percent, setPercent ] = useState(100);

    useEffect(() => {
      const interval = setInterval(() => {
        window.location.reload();
          setRefreshCount(refreshCount => refreshCount + 1);
      }, 10000); // set interval to 10s
      return () => clearInterval(interval);
    }, []);
    useEffect(() => {
      const intervalDrop = setInterval(() => {
          setPercent(percent => percent - 1);
      }, 100); // Set to 2 less 0s than the interval above
      return () => clearInterval(intervalDrop);
    }, []);


    return (
        <div className="Timer">
            <h1>Timer</h1>
            <div className="Timer_Border">
                <div className="Timer_Filler" style={{height: percent + "%"}}></div>
            </div>
        </div>
    )

}

export default Timer;