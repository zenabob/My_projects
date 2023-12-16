import React from "react";
import { View, StyleSheet, Text } from "react-native";
import { useTranslation} from 'react-i18next';
import { TextInput as Input, useTheme, Provider as PaperProvider } from "react-native-paper";
import { DefaultTheme } from 'react-native-paper';

export default function TextInput({ errorText, description, ...props  }) {

  const theme = useTheme();
  const inputTheme = {
    ...DefaultTheme,
    colors: {
      ...DefaultTheme.colors,
      primary: '#00008B', // change border color here
      text: '#00008B', // change label color here
      placeholder: '#00008B', // change placeholder color here
    }
  };

  const {t, i18n} = useTranslation();
  const getTextAlignment = () => {
    const { i18n } = useTranslation();

    if (i18n.language === 'AR' || i18n.language === 'HE') {
      return 'right';
    } else {
      return 'left';
    }
  };
  const textAlignment = getTextAlignment();

  return (
      <View style={styles.container} >
        <PaperProvider theme={inputTheme}>
          <Input
              style={[styles.input, { textAlign: textAlignment }]}
              underlineColor="transparent"
              mode="outlined"
              selectionColor={theme.colors.primary}
              {...props}
          />
        </PaperProvider>
        {description && !errorText ? (
            <Text style={styles.description}>{description}</Text>
        ) : null}
        {errorText ? <Text style={styles.error}>{errorText}</Text> : null}
      </View>
  );
}

const styles = StyleSheet.create({
  container: {
    width: "100%",
    padding: 10,
  },
  input: {
    backgroundColor: 'white',
  },
  description: {
    fontSize: 13,
    color: 'blue',
    paddingTop: 8,
  },
  error: {
    fontSize: 13,
    color: 'red',
    paddingTop: 8,
  },
});
