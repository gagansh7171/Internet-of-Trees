import React from "react";
import {connect} from 'react-redux';
import { withStyles } from "@material-ui/core/styles";
import Box from '@material-ui/core/Box';
import Typography from '@material-ui/core/Typography';
import { Grid } from "@material-ui/core";
import TreeCard from "./treecard"
import axios from 'axios';
import { constants } from "../constant";

const useStyles = (theme) => (
    {
    trees:{
        display:"flex",
        flexDirection:"column",
        alignItems:"center",
        paddingBottom : "20px",
        marginBottom:"14px"
    },
    widthManage:{
        maxWidth : "100%",
    },
    cardHead: {
        backgroundColor : "#d8dee8",//"#f9fafc",
        padding:"10px",
        color : "#1c2d41",
        fontWeight:600,
        fontSize:"24px",
        borderRadius : "6px 6px 0px 0px",
        display:"flex",
        alignItems : "center",
        //border : "1px solid black"
    },
    cardBody: {
        backgroundColor : "white",
        padding:"10px",
        color :"#3c4858",
        borderRadius : " 0px 0px 6px 6px",
        fontSize : "16px",
        fontWeight : 400,
        fontFamily : "arial",
        minHeight : "fit-content",
        marginTop : "1px",
        display: "flex",
        flexDirection: "column",
        //border : "1px solid black"   
    },
    alignCenter :{
        display:"flex",
        justifyContent:"center"
    },
    role:{
        backgroundColor : "#d8dee8",
        color : "#000000",
        fontSize : "13.33px",
        paddingLeft:"10px",
        paddingRight : "10px",
        display:"flex"
    },
    rightSpan : {
        display : "flex",
        flex : "1 1 auto"
    },
    cardWidth:{
        width : 350,
        padding : "5px"
    },
})


class Page extends React.Component{
    constructor(props){
        super(props);
        this.state = {
            ws : null,    
            "All Trees":"0", "Fire Threat" : "1", "Chop Threat" : "2", "Fall Threat" : "3", "Fallen" : "4"
        }
    }
    timeout = 250;
    update = () => {
        axios.get(constants.backend+'trees/').then(
            response => {
                this.setState({trees : response.data})
        }).catch((error) => {
            this.setState({which:'error',load:false})
        });
    }
    check = () => {
        const { ws } = this.state;
        if (!ws || ws.readyState == WebSocket.CLOSED) this.connect(); //check if websocket instance is closed, if so call `connect` function.
    };
    connect = () => {
        var ws = new WebSocket("ws://"+constants.wsp+"ws/tree/");
        let that = this; // cache the this
        var connectInterval;

        // websocket onopen event listener
        ws.onopen = () => {
            this.setState({ ws: ws });
            that.timeout = 250;
            clearTimeout(connectInterval); 
        };

        ws.onmessage = evt => {
            // listen to data sent from the websocket server
            const message = JSON.parse(evt.data)
            // console.log(this.state.trees, message.trees)
            this.setState({"trees": message.trees})
        };

        // websocket onclose event listener
        ws.onclose = e => {
            that.timeout = that.timeout + that.timeout; //increment retry interval
            connectInterval = setTimeout(this.check, Math.min(10000, that.timeout)); //call check function after timeout
        };

        // websocket onerror event listener
        ws.onerror = err => {
            console.error(
                "Socket encountered error: ",
                err.message,
                "Closing socket"
            );
            ws.close();
        };
    }
    componentDidMount(){
        this.update();
        this.connect();
    }    
    deleteTree = (id) => {
        axios.delete(constants.backend+'trees/'+id).then( res => 
            {this.state.ws.send("event");
        });
        // this.setState(prevState => {
        //     return { trees : prevState.trees.filter( (val) => {
        //       return val.id != id;
        //     })};
        //   });
    }
    render(){
        const { classes } = this.props;
        return(
            <>
            <Box className={classes.trees} >
                <Typography variant="h3">
                    <Box display="block" fontFamily="'Lora', serif" fontWeight={700} fontStyle="italic" mb={2}>
                        {this.props.title}
                    </Box>
                </Typography>
                    <Grid container >
                        <Grid item xs={1}></Grid>
                        <Grid item xs={10}>
                        <Grid className={classes.widthManage}  container spacing={2} >
                            {this.state?.trees?.map(entry => 
                                (this.props.title=="All Trees"||this.state[this.props.title]==entry.state)&&<Grid key={entry.macid+entry.nick} item xs={12} md={6} lg={4} className={classes.alignCenter}>
                                    <TreeCard ws={this.state.ws} deleteTree={this.deleteTree} {...entry}/>                 
                                </Grid>)
                            }
                        </Grid>
                        </Grid>
                        <Grid item xs={1}></Grid>
                    </Grid>  
                    
            </Box>
            </>
        );
    }
}

function mapStateToProps(state){
    return {
      currentTheme : state.app.theme || "light",
    };
}

export default withStyles(useStyles)(connect(mapStateToProps, null)(Page));