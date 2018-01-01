import React from "react";
import ReactDOM from "react-dom";

class App extends React.Component {
    render() {
        return (
            // one root element
            <div>
                <h1>Hello!</h1>
            </div>
        );
    }
}

ReactDOM.render(<App/>, window.document.getElementById("app"));
