import * as actionTypes from '../constants';

const initialState = {
    theme : localStorage.getItem("theme") || actionTypes.LIGHT,
};

export const appReducer = (state=initialState, action) => {
    switch (action.type) {
        case actionTypes.SET_THEME :
            localStorage.setItem("theme", action.theme);
            return {
                ...state,
                theme : action.theme
            };
        default:
            return state;
    }
}