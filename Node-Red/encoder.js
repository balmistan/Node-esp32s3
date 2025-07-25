
var str_out = "";
var arr_in = JSON.parse(msg.payload);
var i=0;
var tmp = "";
var offset = 48;

if(arr_in && arr_in.length >0){
    do{
        switch(arr_in[i]['function']){
            case "reset":
                str_out += String.fromCharCode(0+offset);   //0 is the code of reset's command
                break;
            case "pinMode":
                str_out += String.fromCharCode(1+offset);   //1 is the code of pinMode's command
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);   
                str_out += String.fromCharCode( (arr_in[i]['mode'] << 2 ) + offset); 
                break;
            case "digitalRead":
                str_out += String.fromCharCode(2+offset);
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);
                break;
            case "digitalWrite":
                str_out += String.fromCharCode(3+offset);
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);
                str_out += String.fromCharCode(arr_in[i]['value'] + offset);
                break;
            case "analogWrite":
                str_out += String.fromCharCode(4 + offset);
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);
                str_out += String.fromCharCode( (arr_in[i]['value'] & 0x03) + offset);
                str_out += String.fromCharCode((arr_in[i]['value'] >> 2) + offset);
                break;
            case "attachInterrupt":
                str_out += String.fromCharCode(5 + offset);
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);
                str_out += String.fromCharCode((arr_in[i]['mode'] << 2) + offset);
                break;
            case "detachInterrupt":
                str_out += String.fromCharCode(6 + offset);
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);
                break;
            case "digitalToggle":
                str_out += String.fromCharCode(7 + offset);
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);
                break;
            case "ping":
                str_out += String.fromCharCode(8 + offset);
                break;
            case "attachDebounceInterrupt":
                str_out += String.fromCharCode(9 + offset);
                str_out += String.fromCharCode(arr_in[i]['pin'] + offset);
                str_out += String.fromCharCode((arr_in[i]['mode'] << 2) + offset);
                break;
            default:
                break;
        }
        i++
    } while (i < arr_in.length);
}

msg.payload = str_out;

return msg;
