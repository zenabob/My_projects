import React, {useState, useRef} from 'react';
import {useTranslation} from 'react-i18next';
import { FontAwesome } from '@expo/vector-icons';
import End from '../components/End';
import { nameValidator, emailValidator, ContactSubjectValidator, messageValidator } from '../Validators/Validator';
import {
    StyleSheet,
    Text,
    View,
    SafeAreaView,
    Image,
    ScrollView,
    TouchableOpacity,
    Alert,
    Platform,
} from 'react-native';
import { API_URL } from '@env';
import { StatusBar } from 'expo-status-bar';
import TextInput from '../components/TextInput';
import Dropdown from "../components/DropDown";
import axios from 'axios';
import Modal from 'react-native-modal';

 const ContactUsScreen = ({navigation}) => {
    const {t, i18n} = useTranslation();
    const [fullName, setFullName] = useState({ value: "", error: "" });
    const [email, setEmail] = useState({ value: "", error: "" });
    const [subject, setSubject] = useState({ value: "", error: "" });
    const [message, setMessage] = useState({ value: "", error: "" });
    const data = t('contactpage.subjects', { returnObjects: true });
    const [isSuccessModalVisible, setIsSuccessModalVisible] = useState(false);
     // SideBar & Languages
     const [isSideBarOpen, setSideBarOpen] = useState(false);
     const sidebarRef = useRef(null);

     const handleMenuPress = () => {
         setSideBarOpen(!isSideBarOpen);
     }

     const handleLanguageChange = (language) => {
         i18n.changeLanguage(language);
         setSideBarOpen(false);
     };
    
    const handleSend = async () => {
      const nameError = nameValidator(fullName.value);
      const emailError = emailValidator(email.value);
      const messageError = messageValidator(message.value);
      const subjectError = ContactSubjectValidator(subject.value);
      if(nameError || emailError || messageError || subjectError){
        setFullName({ ...fullName, error: nameError });
        setEmail({ ...email, error: emailError });
        setMessage({ ...message, error: messageError });
        setSubject({ ...subject, error: subjectError });
        return;
      }
        try {
          const response = await axios.post(`${API_URL}/contact`, {
            fullName: fullName.value,
            email: email.value,
            subject: subject.value,
            message: message.value,
          });
          console.log(response.data);
        if (response.status === 201) {
            setIsSuccessModalVisible(true);
            // Clear form fields after successful submission
            setFullName({value: '', error: ''});
            setEmail({value: '', error: ''});
            setSubject({value: '', error: ''});
            setMessage({value: '', error: ''});
          } else {
            Alert.alert('Error', 'Failed to send the form. Please try again.');
          }
        } catch (error) {
          console.error(error);
          Alert.alert('Error', 'An error occurred while sending the form. Please try again.');
        }
    };

    const handleCloseSuccessModal = () => {
        setIsSuccessModalVisible(false);
    };

    return (
        <SafeAreaView style={styles.container}>
            <StatusBar style="dark" />
            <View style={styles.sideBarContainer}>
                
                <TouchableOpacity onPress={handleMenuPress} style={styles.menuButton}>
                    {isSideBarOpen ? (
                        <FontAwesome name="times" size={24} color="black"/>
                    ) : (
                        <FontAwesome name="bars" size={24} color="black"/>
                    )}
                </TouchableOpacity>
                {isSideBarOpen && (
                    <ScrollView
                        ref={sidebarRef}
                        contentContainerStyle={styles.sidebarContentContainer}
                    >
                        <View style={styles.languageContainer}>
                            <TouchableOpacity
                                onPress={() => handleLanguageChange('EN')}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>EN</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => handleLanguageChange('AR')}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>AR</Text>
                            </TouchableOpacity>
                            <TouchableOpacity
                                onPress={() => handleLanguageChange('HE')}
                                style={styles.sidebarButton}
                            >
                                <Text style={styles.sidebarButtonText}>HE</Text>
                            </TouchableOpacity>
                        </View>
                        <TouchableOpacity
                            onPress={() => {
                                handleMenuPress();
                                navigation.navigate('SearchScreen');
                            }}
                            style={styles.sidebarButton}
                        >
                            <Text style={styles.sidebarButtonText}>{t('navbar.academic')}</Text>
                        </TouchableOpacity>
                        <TouchableOpacity
                            onPress={() => {
                                handleMenuPress();
                                navigation.navigate('PodcastScreen');
                            }}
                            style={styles.sidebarButton}
                        >
                            <Text style={styles.sidebarButtonText}>{t('navbar.podcast')}</Text>
                        </TouchableOpacity>
                        <TouchableOpacity
                            onPress={() => {
                                handleMenuPress();
                                navigation.navigate('ContactUsScreen');
                            }}
                            style={styles.sidebarButton}
                        >
                            <Text style={styles.sidebarButtonText}>{t('navbar.contact')}</Text>
                        </TouchableOpacity>
                    </ScrollView>
                )}
            </View>
            
            <ScrollView nestedScrollEnabled
              contentContainerStyle={styles.pageContainer}
            //   ref={scrollViewRef}
              showsVerticalScrollIndicator={false}>

            <TouchableOpacity onPress={() => navigation.navigate("StartScreen")} style={styles.logoContainer}>
                <Image
                    source={require("../../assets/FinalLogo.png")}
                    style={styles.logo}
            />
            </TouchableOpacity>
                <View style={styles.title}>
                    <Text style={styles.titleText}>{t('contactpage.title')}</Text>
                    <Text style={styles.titleText}>{t('contactpage.text')}</Text>
                </View>
                <View style={styles.inputsContainer}>
                    <TextInput
                      label={t('contactpage.fullname')}
                      value={fullName.value}
                      onChangeText={(text) => setFullName({ value: text, error: "" })}
                      error={!!fullName.error}
                      errorText={fullName.error}
                    />
                    <TextInput
                      label={t('contactpage.email')} 
                      value={email.value}
                      onChangeText={(text) => setEmail({ value: text, error: "" })}
                      error={!!email.error}
                      errorText={email.error}
                      />
                    <TouchableOpacity style={styles.dropDownContainer}>
                        <Dropdown
                            placeholder={t('contactpage.subject')}
                            label={t('contactpage.subject')}
                            data={data}
                            value={subject.value}
                            setValue={setSubject}
                            setAddedValue={null}
                            errorText={subject.error}
                        />
                    </TouchableOpacity>
                    <TextInput
                      label={t('contactpage.message')}
                      value={message.value}
                      onChangeText={(text) => setMessage({ value: text, error: "" })}
                      error={!!message.error}
                      errorText={message.error}
                      />
                    <TouchableOpacity style={styles.sendButton} onPress={handleSend}>
                        <Text style={styles.sendButtonText}>{t('contactpage.submit')}</Text>
                    </TouchableOpacity>
                </View>
                <Modal isVisible={isSuccessModalVisible}>
                    <View style={styles.modalContainer}>
                      <Text style={styles.modalText}>{t('contactpage.send.title')}</Text>
                      <TouchableOpacity style={styles.modalButton} onPress={handleCloseSuccessModal}>
                        <Text style={styles.modalButtonText}>Close</Text>
                      </TouchableOpacity>
                    </View>
                </Modal>
                <End style={styles.End} navigation={navigation} /> 

      
            </ScrollView>
        </SafeAreaView>
    )

}

export default ContactUsScreen;

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: 'white',
        justifyContent: 'center',
        ...Platform.select({
            android: {
                paddingTop: 40,
            },
        }),
    },
    languageContainer: {
        flexDirection: 'row',
        justifyContent: 'space-between',
    },
    sideBarContainer: {
        flexGrow: 1,
    },
    sidebarContentContainer: {
        flexGrow: 1,
        paddingLeft: 64,
        paddingRight: 64,
        width: '100%',
        height: '100%',
    },
    menuButton: {
        paddingLeft: 16,
        paddingTop: 8,
        height: 40,
    },
    sidebarButton: {
        marginTop: 8,
        marginBottom: 16,
        paddingVertical: 8,
        paddingHorizontal: 16,
        backgroundColor: '#f58723',
        borderRadius: 8,
    },
    sidebarButtonText: {
        alignSelf: 'center',
        fontSize: 16,
        fontWeight: 'bold',
        color: 'white',
    },
    contentContainer: {
        flex: 1,
    },
    inputsContainer: {
        paddingTop: 20,
    },
    title: {
        marginTop: 10,
    },
    titleText: {
        alignSelf: 'center',
        fontWeight: 'bold',
        fontSize: 16,
    },
    logoContainer: {
        width: 200,
        height: 75,
        alignSelf: 'center', 
        resizeMode: "contain",
    },
    logo: {
        width: 200,
        height: 75,
        alignSelf: 'center', 
        resizeMode: "contain",
    },
    sendButton: {
        backgroundColor: "green",
        borderRadius: 8,
        paddingVertical: 12,
        paddingHorizontal: 16,
        width: '50%',
        alignSelf: 'center', 
        marginTop: 20, 
        alignItems: 'center',
    },
    sendButtonText: {
        fontSize: 16,
        fontWeight: "bold",
        color: "white",
    },
    modalContainer: {
        backgroundColor: 'white',
        padding: 16,
        alignItems: 'center',
        justifyContent: 'center',
        borderRadius: 8,
      },
      modalText: {
        fontSize: 20,
        fontWeight: 'bold',
        marginBottom: 16,
      },
      modalButton: {
        backgroundColor: 'green',
        paddingVertical: 8,
        paddingHorizontal: 16,
        borderRadius: 8,
      },
      modalButtonText: {
        color: 'white',
        fontSize: 16,
        fontWeight: 'bold',
      },
    //   pageContainer: {
    //     flexGrow: 1,
    //     alignItems: "center",
    //     paddingBottom: 20,
    // },
});
