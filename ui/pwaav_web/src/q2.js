import React from 'react';
import {
    Button,
    TextField,
    Card,
    Grid,
    CardContent,
    CardMedia,
    Typography,
    CardActions,

} from '@material-ui/core';

import {
    BrowserRouter as Router,
    Switch,
    Route,
    Link
} from "react-router-dom";



class Q2 extends React.Component {
    constructor(props) {
        var pictures = ["https://i.kym-cdn.com/entries/icons/mobile/000/018/929/graphic_design_is_my_passion.jpg",
            "https://media.altchar.com/prod/images/940_530/gm-daad6589-7a29-410d-8381-f04b0d70257b-oldschoolrunescapecomestoiosandandroid.jpeg",
            "https://i.kym-cdn.com/photos/images/newsfeed/001/018/899/936.jpg", "https://ih1.redbubble.net/image.851993396.0965/fposter,small,wall_texture,product,750x1000.jpg"
        ];
        super(props);
        this.state = {
            showPicture: Math.floor(Math.random() * Math.floor( pictures.length)),
            pictureArray:  pictures,

        }
    }


    render() {
        return (
            <Grid container>
                <Grid item xs={12} xl={6} >
                    <Card>
                        <CardMedia 
                        square
                            image={ this.state.pictureArray[ this.state.showPicture ] }
                            style={{ height: "100vh" }}>
                        </CardMedia></Card>
                </Grid>
                <LoginPage questionID={this.props.questionID} ></LoginPage>

            </Grid>);

    }
}

class LoginPage extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            password: "",

        }

        this.handleSubmit = this.handleSubmit.bind(this);

    }

    async handleSubmit(event) {
        event.preventDefault();
        var res = await fetch('http://127.0.0.1:8000/', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                "function": "question",
                "args": {
                    "questionID": this.props.questionID,
                    "answer": this.state.password,



                }
            })
        });
        var response = await res.json();
        this.setState({ statusMessage: response.message });
        if (response.answer) {
            this.setState({ nextID: response.nextID });
        }
        else {

        }


    }
    render() {
        return (
            <Grid item xs={12} xl={6} >
                <form
                    style={{ height: "100vh" }}
                    onSubmit={this.handleSubmit}
                >

                    <Grid container >
                        <Grid xl={2} item xs={2}></Grid>
                        <Grid xl={8} item xs={8}>
                            <Card style={{ margin: 50 }}>
                                <CardContent  >
                                    <Typography variant="h5" component="h1" >
                                        Log in
                    </Typography>
                                    <Typography variant="h5" component="h1" >
                                        Username:
                    </Typography>
                                    <TextField disabled defaultValue="Vamsi"> </TextField>

                                </CardContent>
                                <CardContent>
                                    <Typography variant="h5" component="h1" >
                                        Password:
                    </Typography>
                                    <TextField
                                        value={this.state.password}
                                        onChange={(event) => {
                                            this.setState({ password: event.target.value })
                                        }}
                                    > </TextField>


                                </CardContent>
                                <CardActions style={{ justifyContent: 'space-between' }}  >
                                    <Button

                                        onClick={this.handleSubmit}
                                        variant="contained" color="primary" >Log in</Button>


                                    {(this.state.nextID) &&

                                        (

                                            <Link to={this.state.nextID}>Next</Link>
                                        )}

                                </CardActions>

                            </Card>
                        </Grid>
                        <Grid xl={2} item xs={2}></Grid>

                    </Grid>

                </form>


            </Grid>
        );

    }
}

export default Q2;