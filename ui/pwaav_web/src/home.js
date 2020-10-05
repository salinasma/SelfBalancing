
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
  Text

} from '@material-ui/core';
import {
  BrowserRouter as Router,
  Switch,
  Route,
  Link
} from "react-router-dom";



class Home extends React.Component {
    render() {
        return ( <div>
          <p> Welcome to your challenge Vamsi  potatoe </p>
          <Link   to="c19c410d-e56c-4f61-a271-10bdb5c87f58" >

            <Button variant="contained" color="primary" > Start </Button>
          </Link>
           </div>
        );

    }
}

export default Home;