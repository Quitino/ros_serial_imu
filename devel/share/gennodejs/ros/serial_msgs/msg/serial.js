// Auto-generated. Do not edit!

// (in-package serial_msgs.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class serial {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.serial = null;
    }
    else {
      if (initObj.hasOwnProperty('serial')) {
        this.serial = initObj.serial
      }
      else {
        this.serial = [];
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type serial
    // Serialize message field [serial]
    bufferOffset = _arraySerializer.uint8(obj.serial, buffer, bufferOffset, null);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type serial
    let len;
    let data = new serial(null);
    // Deserialize message field [serial]
    data.serial = _arrayDeserializer.uint8(buffer, bufferOffset, null)
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.serial.length;
    return length + 4;
  }

  static datatype() {
    // Returns string type for a message object
    return 'serial_msgs/serial';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '91402c4025b67ddb569b7ca2fb02ac79';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8[] serial
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new serial(null);
    if (msg.serial !== undefined) {
      resolved.serial = msg.serial;
    }
    else {
      resolved.serial = []
    }

    return resolved;
    }
};

module.exports = serial;
