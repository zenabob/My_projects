import React, {useEffect, useState, useRef} from 'react';
import {FontAwesome} from '@expo/vector-icons';
import {useTranslation} from 'react-i18next';
import LanguageContext from '../components/LanguageContext';
import {I18nManager, Platform, TouchableWithoutFeedback} from "react-native";
import { StatusBar } from 'expo-status-bar';
import axios from 'axios';
import { API_URL } from '@env';
import {
    StyleSheet,
    Text,
    View,
    SafeAreaView,
    Image,
    ScrollView,
    TouchableOpacity,
    Dimensions,
    // Platform,
} from 'react-native';
import Dropdown from "../components/DropDown";
import MyModal from '../components/Modal'
import Modal from 'react-native-modal';
import End from '../components/End';
import { getCityList } from '../lists/list';
import {DataToSelectOptions} from "../components/HelperFunction";

const window = Dimensions.get('window');


export default function SearchScreen({navigation}) {
    const {t, i18n} = useTranslation();
    const getTextAlignment = () => {
        const { i18n } = useTranslation();
      
        if (i18n.language === 'AR' || i18n.language === 'HE') {
          return 'right'; // Right-to-left alignment for Spanish
        } else {
          return 'left'; // Left-to-right alignment for other languages
        }
    };
    const textAlignment = getTextAlignment();
    const [modalVisible, setModalVisible] = useState(false);
    const [academics, setAcademics] = useState([]);
    const [city, setCity] = useState({value: "", error: ""});
    const [subject, setSubject] = useState([]);
    const [subjectData,setSubjectData] = useState({value:"",error:""})
    const [isConfirmed, setIsConfirmed] = useState(false);
    const [errorResponse, setErrorResponse] = useState(false);
    const [message, setMessage]  = useState('');
    const toggleModal = () => {
        setModalVisible(!modalVisible);
    }
    const subjectOptions = DataToSelectOptions(subject,'subject','subject')
    const cities = getCityList;
    useEffect(() => {
        fetchAcademics();
    }, []);
    const fetchAcademics = async () => {
        try {
            const response = await axios.get(`${API_URL}/academic/isApproved/true`);
            const subjects = await axios.get(`${API_URL}/academic/subjects`);
            setSubject(subjects.data);
            setAcademics(response.data);
        } catch (error) {
            console.error(error);
        }
    };

    const renderItem = ({item}) => (
        <View style={styles.card} key={item.id}>
            <Image style={styles.image} source={{uri: item.imageUrl}}/>
            <View style={styles.detailsContainer}>
                <Text style={styles.fullName}>{`${item.firstName} ${item.lastName}`}</Text>
                <Text style={styles.email}>{item.email}</Text>
                <Text style={styles.city}>{item.city}</Text>
                <Text style={styles.degree}>{`${item.degree} : ${item.subject.subject}`}</Text>
                <Text style={styles.degree}>{`${item.career.career}`}</Text>
            </View>
        </View>
    );

    const filteredData = academics.filter((item) => {
        if (city?.value && subjectData?.value) {
            return item.city === city?.value && item.subject.subject === subjectData?.value;
        } else if (city?.value !== '') {
            return item.city === city?.value ;
        } else if (subjectData?.value !== '') {
            return item.subject.subject === subjectData?.value;
        }
        return true;
    });

    const [isSideBarOpen, setSideBarOpen] = useState(false);
    const sidebarRef = useRef(null);

    const handleMenuPress = () => {
        setSideBarOpen(!isSideBarOpen);
    }

    const handleLanguageChange = (language) => {
        i18n.changeLanguage(language);
        setSideBarOpen(false);
    };
    return (
        <SafeAreaView style={styles.container}>
            <StatusBar barStyle="dark-content"/>
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
            <ScrollView Style={styles.contentContainer}>
                <TouchableOpacity onPress={() => navigation.navigate("StartScreen")} style={styles.logoContainer}>
                    <Image
                        source={require("../../assets/FinalLogo.png")}
                        style={styles.logo}
                    />
                </TouchableOpacity>
                <View style={styles.slideTextContainer}>
                    <Text style={styles.slideTitle}>{t('academicpage.title')}</Text>
                    <Text style={styles.slideText}>{t('academicpage.text')}</Text>
                </View>

                <TouchableOpacity onPress={toggleModal} style={styles.joinButton}>
                    <Text style={styles.joinButtonText}>{t('homepage.joinus')}</Text>
                </TouchableOpacity>
                <MyModal 
                    modalVisible={modalVisible} 
                    toggleModal={toggleModal} 
                    setIsConfirmed={setIsConfirmed} 
                    setMessage={setMessage}
                    setErrorResponse={setErrorResponse}
                />
                <Text style={[styles.label,{ textAlign: textAlignment }]}>{t('academicpage.acdemics')}</Text>
                <View style={styles.dropDownContainer}>
                    <TouchableOpacity>
                        <Dropdown
                            placeholder={t('academicpage.acdemicsField')}
                            label={t('academicpage.acdemicsField')}
                            data={subjectOptions}
                            value={subjectData.value}
                            setValue={setSubjectData}
                            setAddedValue={null}
                            errorText={subjectData.error}
                        />
                        <Dropdown
                            placeholder={t('academicpage.acdemics.Area')}
                            label={t('academicpage.acdemics.Area')}
                            data={cities.map((city) => ({
                                label: city.label,
                                value: city.label,
                              }))}
                            value={city.value}
                            setValue={setCity}
                            setAddedValue={null}
                            errorText={city.error}
                        />
                    </TouchableOpacity>

                </View>

                <View style={styles.pageContent}>
                    {filteredData.map((item) => renderItem({item}))}
                </View>
               
                <End style={styles.end} navigation={navigation}/>

            </ScrollView>
            <Modal isVisible={isConfirmed}>
                <View style={styles.modalContainer}>
                    <Text style={styles.modalText}>{message}</Text>
                      <TouchableOpacity style={errorResponse? styles.modalButtonError : styles.modalButton} onPress={() => setIsConfirmed(false)}>
                        <Text style={styles.modalButtonText}>Close</Text>
                    </TouchableOpacity>
                </View>
            </Modal>
        </SafeAreaView>
    );
}

const styles = StyleSheet.create({
    container: {
        flex: 1,
        backgroundColor: "white",
        flexGrow: 1,
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
        flexGrow: 1,
        alignItems: "center",
        paddingBottom: 20,

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
        resizeMode: "contain",
        alignSelf: 'center',
    },
    joinButton: {
        backgroundColor: "#041041",
        borderRadius: 8,
        paddingVertical: 12,
        paddingHorizontal: 16,
        alignSelf: 'center',

    },
    joinButtonText: {
        fontSize: 16,
        fontWeight: "bold",
        color: "white",
        padding: 10,
    },
    slideTextContainer: {
        marginTop: 10,
        alignItems: 'center',
        padding: 10,

    },
    label: {
        paddingTop: 16,
        fontSize: 22,
        color: 'darkorange',
        fontWeight: "bold",
        padding: 20,
    },
    dropDownContainer: {
        paddingTop: 16,
    },
    dropDown: {
        width: '100%',
    },
    sliderContainer: {
        height: Dimensions.get("window").height / 3,
        marginTop: 10,
        alignItems: 'center',

    },
    slideTitle: {
        paddingVertical: 10,
        fontSize: 25,
        fontWeight: "bold",
        color: '#041041',
        padding: 10,
        borderColor: "blue",
        textAlign: "center",
    },
    slideText: {
        paddingVertical: 10,
        fontSize: 20,
        fontWeight: "bold",
        color: '#041041',
        padding: 10,
        borderColor: "blue",
        textAlign: "center",
    },
    end: {
        width: Dimensions.get("window").width,
        backgroundColor: '#092D82',
        flexDirection: 'row',
        alignSelf: 'flex-start',
        bottom: 0,
        position: 'absolute',
    },

    card: {
        flexDirection: 'column',
        marginBottom: 10,
        backgroundColor: '#fff',
        borderRadius: 8,
        padding: 10,
        shadowColor: '#000',
        shadowOffset: {width: 0, height: 2},
        shadowOpacity: 0.2,
        elevation: 2,
        width: '70%',
        height: 380,
        alignSelf: 'center'
    },
    image: {
        width: '80%',
        height: '60%',
        alignSelf: 'center',
        borderRadius: 15,
        marginBottom: 10,
    },
    detailsContainer: {
        flex: 1,
        alignItems: 'center',
    },
    fullName: {
        fontSize: 18,
        fontWeight: 'bold',
        marginBottom: 5,
    },
    email: {
        fontSize: 16,
        marginBottom: 5,
    },
    city: {
        fontSize: 14,
        marginBottom: 5,
    },
    degree: {
        fontSize: 12,
        color: 'gray',
    },

    pageContent: {
        flex: 1,
        alignItems: 'center',
        justifyContent: 'center',
        padding: 16,
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
      modalButtonError: {
        backgroundColor: 'red',
        paddingVertical: 8,
        paddingHorizontal: 16,
        borderRadius: 8,
      },
      modalButtonText: {
        color: 'white',
        fontSize: 16,
        fontWeight: 'bold',
      },
})