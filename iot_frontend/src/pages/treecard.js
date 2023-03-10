import React from "react";
import {connect} from 'react-redux';
import { withStyles } from "@material-ui/core/styles";
import EditOutlinedIcon from '@material-ui/icons/EditOutlined';
import DeleteOutlineOutlinedIcon from '@material-ui/icons/DeleteOutlineOutlined';
import { IconButton, Modal, Typography, Input, Box, Button } from "@material-ui/core";
import axios from 'axios'
import { constants } from "../constant";

const useStyles = (theme) => (
{
    cardWidth:{
        width : 350,
        padding : "7px",
        borderRadius : "4px",
        position : "relative",
        minHeight : "100px",
        paddingBottom:"50px",
        // height: "fit-content",
    },
    cardHead:{
        display: "flex",
        justifyContent: "space-between",
    },
    normal:{
        backgroundColor: "#43a047",
        color: "white"
    },
    danger:{
        backgroundColor : "#e53935",
        color: "white"
    },
    fallen:{
        backgroundColor: "#546e7a",
        color: "white"
    },
    nick:{
        fontWeight : 500,
        fontSize : "large",
        width: "80%",
        overflowWrap : "break-word",
    },
    buttonStyle:{
        padding : "5px !important",
        color : "white",
        height : "fit-content",
    },
    desc:{
        position: "absolute",
        bottom : "7px"
    },
})
const modal = {
    position: 'absolute',
    top: '50%',
    left: '50%',
    transform: 'translate(-50%, -50%)',
    width: 400,
    bgcolor: 'background.paper',
    border: '2px solid #000',
    boxShadow: 24,
    p: 4,
  };

class TreeCard extends React.Component{
    constructor(props){
        super(props);
        this.state = { nicknameform:"", openModal : false, 
                        stateVal : {"0" : "Normal", "1" : "Fire Threat", "2" : "Chop Threat", "3" : "Fall Threat","4": "Fallen"},
                    ...props};
        if(this.props.state == "0"){
            this.state["bgcolor"] =  "normal";
        }else if(this.props.state == "4"){
            this.state["bgcolor"] = "fallen";
        }else{
            this.state["bgcolor"] = "danger";
        }
    }

    editNick = (e) => {
        if(this.state.nicknameform.length > 0)
        axios.put(constants.backend+'trees/'+this.props.id+"/", {"nick" : this.state.nicknameform})
        .then(res => {
            this.props.ws.send("event");
            this.setState(prevState => { return {openModal : false, nick : prevState.nicknameform, nicknameform: ""}});
        });
        this.setState({openModal : false});
        
    }
    handleOpen = () => {
        this.setState({openModal : true});
    }
    handleClose = () => {
        this.setState({openModal : false});
    }
    render(){
        const { classes } = this.props;
        return(
            <>
                
                <Box className={[classes.cardWidth, (classes[this.state.bgcolor])].join(' ')}>
                    <Box className={classes.cardHead}>
                        <span className={classes.nick}>{this.state.nick}</span>
                        <span className={classes.cardHead}>
                            <IconButton className={classes.buttonStyle} onClick={this.handleOpen}><EditOutlinedIcon/></IconButton> 
                            <IconButton className={classes.buttonStyle} onClick={() => this.props.deleteTree(this.props.id)}><DeleteOutlineOutlinedIcon/></IconButton>
                            <Modal open={this.state.openModal} onClose={this.handleClose}>
                                <Box sx={modal}>
                                <Typography id="modal-modal-title" variant="h6" component="h2">
                                    Set new nick-name for this tree
                                </Typography>
                                <Input 
                                    placeholder="New Nick Name" 
                                    onChange={(e) => this.setState({nicknameform : e.target.value})} 
                                    value={this.state.nicknameform} name="nick-name" 
                                    inputProps={{ maxLength: 25 }} 
                                    style={{marginLeft:"10px", marginTop:"3px", width:"32ch", marginBottom: "10px"}}/>
                                <Button variant="contained" color="primary" onClick={this.editNick}>
                                    Submit
                                </Button>
                                </Box>
                            </Modal>
                        </span>
                    </Box>         
                    <Box className={classes.desc}>
                        {this.state.macid}<br/>
                        {this.state.stateVal[this.state.state]}
                    </Box>
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

export default withStyles(useStyles)(connect(mapStateToProps, null)(TreeCard));