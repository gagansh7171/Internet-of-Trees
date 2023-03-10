import * as actionTypes from '../constants';
export const setTheme = (theme) => {
    return{
        type : actionTypes.SET_THEME,
        theme : theme
    }
};