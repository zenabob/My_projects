import React, { useState } from 'react';
import { StyleSheet, Text, View, TouchableOpacity } from 'react-native';
import { Dropdown } from 'react-native-element-dropdown';
import AntDesign from '@expo/vector-icons/AntDesign';

const DropdownComponent = ({ placeholder, label, data, value, setValue, errorText, setAddedValue}) => {
  const [isFocus, setIsFocus] = useState(false);

  const handleCancel = () => {
    setValue({value:'', error: ''});
    if(setAddedValue) setAddedValue('');
  };

  const renderLabel = () => {
    if (value || isFocus) {
      return (
        <Text style={[styles.label, isFocus && styles.focusedLabel]}>
          {label}
        </Text>
      );
    }
    return null;
  };

  return (
    <View
        style={styles.container}
        underlineColor="transparent"
        mode="outlined"
    >
      <Dropdown
        style={[errorText? styles.errorDropDown : styles.dropdown, isFocus && styles.focusedDropdown]}
        placeholderStyle={errorText? styles.errorPh : styles.placeholderStyle}
        selectedTextStyle={styles.selectedTextStyle}
        inputSearchStyle={styles.inputSearchStyle}
        iconStyle={styles.iconStyle}
        data={data}
        search
        maxHeight={300}
        labelField="label"
        valueField="value"
        placeholder={value ? value : !isFocus ? placeholder || 'Select item' : '...'}
        searchPlaceholder="Search..."
        value={value}
        onFocus={() => setIsFocus(true)}
        onBlur={() => setIsFocus(false)}
        onChangeText={(item)=> {
          if(setAddedValue !== null) 
            setAddedValue(item);
        }}
        onChange={(item) => {
          setValue({value: item.label, error: ""});
          setIsFocus(false);
        }}
        renderLeftIcon={() => (
          <AntDesign
            style={styles.icon}
            color={isFocus ? 'blue' : 'black'}
            name="Safety"
            size={20}
          />
        )}
      />
      {renderLabel()}
      {value && (
        <TouchableOpacity onPress={handleCancel} style={styles.closeButton}>
          <AntDesign name="close" size={16} color="gray"/>
        </TouchableOpacity>
      )}
      {errorText ? <Text style={styles.error}>{errorText}</Text> : null}
    </View>
  );
};

export default DropdownComponent;

const styles = StyleSheet.create({
  container: {
    padding: 10,
    width: '100%',
  },
  dropdown: {
    height: 50,
    backgroundColor: 'white',
    borderWidth: 1,
    borderColor: 'gray',
    borderRadius: 5,
    paddingHorizontal: 8,
  },
  errorDropDown: {
    height: 50,
    backgroundColor: 'white',
    borderWidth: 2,
    borderColor: '#8b0000',
    borderRadius: 5,
    paddingHorizontal: 8,
  },
  focusedDropdown: {
    borderColor: '#00008B',
    borderWidth: 1.5,
  },
  icon: {
    marginRight: 5,
  },
  label: {
    position: 'absolute',
    backgroundColor: 'white',
    left: 18,
    top: 0,
    zIndex: 1,
    paddingHorizontal: 8,
    fontSize: 14,
    color: '#00008B',
  },
  focusedLabel: {
    top: 0,
    color: '#00008B',
  },
  placeholderStyle: {
    fontSize: 16,
  },
  errorPh: {
    color: '#8b0000',
  },
  selectedTextStyle: {
    fontSize: 16,
  },
  iconStyle: {
    width: 20,
    height: 20,
  },
  inputSearchStyle: {
    height: 40,
    fontSize: 16,
  },
  error: {
    fontSize: 13,
    color: 'red',
    paddingTop: 8,
  },
  closeButton: {
    paddingLeft: 10,
  },
});
