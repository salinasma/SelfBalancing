# pull official base image
FROM node:13.12.0-alpine

# set working directory
WORKDIR /app

# add `/app/node_modules/.bin` to $PATH
ENV PATH /app/node_modules/.bin:$PATH

# install app dependencies
COPY package.json ./
RUN npm install --silent
RUN npm install react-scripts@3.4.1 -g --silent
RUN npm install @material-ui/core
RUN npm install react-router-dom
RUN npm install react-highlight  --save

# add app
COPY . ./

# start app
CMD ["npm", "start"]
