// Auto-generated. Do not edit!

// (in-package relative_angle_nb.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class Apl_kai {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.obs_dist = null;
      this.obs_direct = null;
      this.goal_x = null;
      this.goal_y = null;
      this.obs_latelitude = null;
    }
    else {
      if (initObj.hasOwnProperty('obs_dist')) {
        this.obs_dist = initObj.obs_dist
      }
      else {
        this.obs_dist = 0;
      }
      if (initObj.hasOwnProperty('obs_direct')) {
        this.obs_direct = initObj.obs_direct
      }
      else {
        this.obs_direct = 0.0;
      }
      if (initObj.hasOwnProperty('goal_x')) {
        this.goal_x = initObj.goal_x
      }
      else {
        this.goal_x = 0.0;
      }
      if (initObj.hasOwnProperty('goal_y')) {
        this.goal_y = initObj.goal_y
      }
      else {
        this.goal_y = 0.0;
      }
      if (initObj.hasOwnProperty('obs_latelitude')) {
        this.obs_latelitude = initObj.obs_latelitude
      }
      else {
        this.obs_latelitude = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type Apl_kai
    // Serialize message field [obs_dist]
    bufferOffset = _serializer.uint16(obj.obs_dist, buffer, bufferOffset);
    // Serialize message field [obs_direct]
    bufferOffset = _serializer.float32(obj.obs_direct, buffer, bufferOffset);
    // Serialize message field [goal_x]
    bufferOffset = _serializer.float32(obj.goal_x, buffer, bufferOffset);
    // Serialize message field [goal_y]
    bufferOffset = _serializer.float32(obj.goal_y, buffer, bufferOffset);
    // Serialize message field [obs_latelitude]
    bufferOffset = _serializer.int32(obj.obs_latelitude, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type Apl_kai
    let len;
    let data = new Apl_kai(null);
    // Deserialize message field [obs_dist]
    data.obs_dist = _deserializer.uint16(buffer, bufferOffset);
    // Deserialize message field [obs_direct]
    data.obs_direct = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [goal_x]
    data.goal_x = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [goal_y]
    data.goal_y = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [obs_latelitude]
    data.obs_latelitude = _deserializer.int32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 18;
  }

  static datatype() {
    // Returns string type for a message object
    return 'relative_angle_nb/Apl_kai';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '9809d650616fd3c284b0b4b27cd2e9fe';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint16 obs_dist
    float32 obs_direct
    float32 goal_x
    float32 goal_y
    int32 obs_latelitude
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new Apl_kai(null);
    if (msg.obs_dist !== undefined) {
      resolved.obs_dist = msg.obs_dist;
    }
    else {
      resolved.obs_dist = 0
    }

    if (msg.obs_direct !== undefined) {
      resolved.obs_direct = msg.obs_direct;
    }
    else {
      resolved.obs_direct = 0.0
    }

    if (msg.goal_x !== undefined) {
      resolved.goal_x = msg.goal_x;
    }
    else {
      resolved.goal_x = 0.0
    }

    if (msg.goal_y !== undefined) {
      resolved.goal_y = msg.goal_y;
    }
    else {
      resolved.goal_y = 0.0
    }

    if (msg.obs_latelitude !== undefined) {
      resolved.obs_latelitude = msg.obs_latelitude;
    }
    else {
      resolved.obs_latelitude = 0
    }

    return resolved;
    }
};

module.exports = Apl_kai;
