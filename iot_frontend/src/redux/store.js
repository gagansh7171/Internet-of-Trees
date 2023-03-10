import { applyMiddleware, compose, createStore } from "redux";
import thunk from "redux-thunk";
import { rootReducer } from "./reducers";


// devtools for debugging in dev environment.
const devTools =
  // eslint-disable-next-line no-undef
  process.env.NODE_ENV !== "production" && window.__REDUX_DEVTOOLS_EXTENSION__
    ? window.__REDUX_DEVTOOLS_EXTENSION__ &&
      window.__REDUX_DEVTOOLS_EXTENSION__()
    : (a) => a;

export const store = createStore(
  rootReducer,
  compose(applyMiddleware(thunk), devTools)
);