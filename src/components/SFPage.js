//
//  // Smart Fan Website.
//

// SMART FAN TEMPLATE
import React from 'react';
// USE THIS FOR API
import axios from 'axios';
// import fanSubscriber from '../../subcriptions/fanSubscription';

class SMPage extends React.Component{

  constructor(props){
    super(props);

    this.state = {
      onCommand: '',
      data: [],
      duration: 0,
      ledID: 0,
      power: '0',
      price: '0'
    }

    this.socket = io();

    // Subscription to Temp
    this.socket.on('exampleDataRecieved', (temp) => {
      console.log(temp);
      if (parseFloat(temp) >= parseFloat(this.state.setTemp)){
        this.setState({
          currentTemp: temp,
          status: 'on'
        })
      } else {
        this.setState({
          currentTemp: temp,
          status: 'off'
        })
      }
    });

    // this.socket.emit()
    this.socket.emit('testExample');

    this.socket.on('duration', (data) => {
      console.log('website sent me data', data)
      this.setState(() => ({
        duration: data.duration
      }));
    });


    // Fetch all fan data from database
    axios.get('/led')
    .then((data) => {
       console.log('response ', data);
          this.setState(() => ({
            data: data.data
          }));
    })

  }

  // Update Database, Post new fan data
  onDBpush = (e) => {
    e.preventDefault();
    this.socket.emit('on', {my: 'data'});
    console.log('html toggle firing');

    // Example of pushing data to the database
    axios.post('/fan', {
      temp: this.state.onCommand,
      status: this.state.onCommand
    });

    // Example of fetching data from database
    axios.get('/fan')
    .then((response) => {
       console.log('response ', response.data);  // This is the data we receive !!

          // Within this example we are actually save the data we recieve to the current state
          this.setState(({
            temps: response.data
          }));
    })
  }

  onTextInput = (e) => {
    const input = e.target.value;
    this.setState(() => ({
        onCommand: input
      }));
  }

  onChangePower = (e) => {
    const power = e.target.value;
    this.setState(() => ({power}))

  }

  onChangePrice = (e) => {
    const price = e.target.value;
    this.setState(() => ({price}))
  }

  onLockLed = (e) => {
    e.preventDefault();
    this.socket.emit('lock', {lock: true});
  }

  onUnLockLed = (e) => {
    e.preventDefault();
    this.socket.emit('unlock', {unlock: false});
  }

  onLedSelected = (e) => {
    const ledID = e.target.value;
    console.log('ledID', ledID);
    this.setState(({
      ledID
    }));
  }

  // Send Command to begin taking temprature data
  onCommand = (e) => {
    e.preventDefault();
    this.socket.emit('command', {command: this.state.onCommand});
  }

  /* eslint-disable */
  // Render HTML
  render() {
    return (
      <div>
                <h1>Smart Light</h1>
                <p class ="abc">created by : JunYan Low --101821189</p>
                <p class="abc">duration: {this.state.duration} </p>
                <p class="abc">price (5$ per kw): ${(this.state.duration / 360) * (parseInt(this.state.power)) * (parseInt(this.state.price)) } </p>
          <section>
          <div>
                  <button class="button" onClick={this.onLockLed}>lock motion sensor</button>
                </div>
                <div>
                  <button class="button" onClick={this.onUnLockLed}>unlock motion sensor</button>
                </div>
                <div>
                <select onChange={this.onLedSelected}>
                  {
                    this.state.data.map(led => (
                        <option key={led._id} value={led.duration}>{led._id}</option>
                    ))
                  }
                </select>
                </div>

                <input onChange={this.onChangePower} value={this.state.power} type="text"/>
                <input onChange={this.onChangePrice} value={this.state.price} type="text"/>
          </section>

          <section class="data">
          {
            this.state.data.map((data) =>
                <div key={data._id}>
                  <p>_id: {data._id}</p>
                  <p>date: {data.date}</p>
                  <p>duration: {data.duration}</p>
                </div>
            )
          }
          </section>
        </div>
    )
  }
}

export default SMPage;